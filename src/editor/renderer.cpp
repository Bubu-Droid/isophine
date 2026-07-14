// TODO: Learn PDF fundamentals from https://opensource.adobe.com/dc-acrobat-sdk-docs/pdfstandards/pdfreference1.7old.pdf
// This is the only scope in the entire project where I had to heavily rely on AI.
// I don't know shit about PDF fundamentals -- I tried reading the document above but was overwhelmed by its volume.
// The same goes for QPDF's docs.
// NOTE: If you know how QPDF / PDF internals work and see something off in this file, please let me know.
// I'll be working on this script during the next inter-semestral break, so, I'd hopefully be able to learn
// more about PDF internals and QPDF by then.

#include "renderer.h"

#include <QGuiApplication>
#include <QScreen>
#include <cmath>
#include <iostream>
#include <qpdf/QPDF.hh>
#include <qpdf/QPDFObjectHandle.hh>
#include <qpdf/QPDFPageDocumentHelper.hh>
#include <qpdf/QPDFPageObjectHelper.hh>
#include <qpdf/QPDFWriter.hh>
#include <sstream>

// TODO: remove this and make a project_data.h file with only this struct

static std::string fmtNum(double v) {
  std::ostringstream oss;
  oss.precision(6);
  oss << std::fixed << v;
  return oss.str();
}

std::string PDFRenderer(
    const QString& inputPdfPath,
    const QString& outputPdfPath,
    const std::vector<PageTransform>& pageTransformVector,
    const QColor& bgColor,
    const QSizeF& pageDimensions
) {
  try {
    QPDF qpdf;
    qpdf.processFile(inputPdfPath.toUtf8().constData());

    QPDFPageDocumentHelper srcDh(qpdf);
    std::vector<QPDFPageObjectHelper> srcPages = srcDh.getAllPages();
    size_t pageCount = srcPages.size();

    QPDF outpdf;
    outpdf.emptyPDF();
    QPDFPageDocumentHelper outDh(outpdf);

    for (int i = 0; i < static_cast<unsigned>(pageCount); ++i) {
      QPDFPageObjectHelper& srcPage = srcPages[i];
      QPDFObjectHandle pageDict = srcPage.getObjectHandle();

      QPDFObjectHandle box = pageDict.hasKey("/CropBox")
          ? pageDict.getKey("/CropBox")
          : pageDict.getKey("/MediaBox");

      double px0 = box.getArrayItem(0).getNumericValue();
      double py0 = box.getArrayItem(1).getNumericValue();
      double px1 = box.getArrayItem(2).getNumericValue();
      double py1 = box.getArrayItem(3).getNumericValue();
      double page_w = px1 - px0;
      double page_h = py1 - py0;

      QPDFObjectHandle xobj = srcPage.getFormXObjectForPage(true);

      QPDFObjectHandle xobjInOut = outpdf.copyForeignObject(xobj);

      double theta = -pageTransformVector[i].rotationAmount * M_PI / 180.0;
      double s = pageTransformVector[i].scaleAmount;
      double cosT = std::cos(theta);
      double sinT = std::sin(theta);

      double a = s * cosT;
      double b = s * sinT;
      double c = -s * sinT;
      double d = s * cosT;

      double cx = page_w / 2.0;
      double cy = page_h / 2.0;

      double e = pageTransformVector[i].xOffset
              * (72.0
                 / QGuiApplication::primaryScreen()->physicalDotsPerInchX())
          + pageDimensions.width() / 2.0 - (a * cx + c * cy);
      double f = -pageTransformVector[i].yOffset
              * (72.0
                 / QGuiApplication::primaryScreen()->physicalDotsPerInchX())
          + pageDimensions.height() / 2.0 - (b * cx + d * cy);

      QPDFObjectHandle newPage = QPDFObjectHandle::parse(
          "<< /Type /Page /MediaBox [0 0 " + fmtNum(pageDimensions.width())
          + " " + fmtNum(pageDimensions.height()) + "] >>"
      );

      QPDFObjectHandle resources =
          QPDFObjectHandle::parse("<< /XObject << >> >>");
      resources.getKey("/XObject").replaceKey("/SrcPage", xobjInOut);
      newPage.replaceKey("/Resources", resources);

      std::ostringstream content;
      content << "q\n"
              << fmtNum(bgColor.redF()) << " " << fmtNum(bgColor.greenF())
              << " " << fmtNum(bgColor.blueF()) << " rg\n"
              << "0 0 " << fmtNum(pageDimensions.width()) << " "
              << fmtNum(pageDimensions.height()) << " re\n"
              << "f\n"
              << "Q\n"
              << "q\n"
              << "0 0 " << fmtNum(pageDimensions.width()) << " "
              << fmtNum(pageDimensions.height()) << " re W n\n"
              << fmtNum(a) << " " << fmtNum(b) << " " << fmtNum(c) << " "
              << fmtNum(d) << " " << fmtNum(e) << " " << fmtNum(f) << " cm\n"
              << "/SrcPage Do\n"
              << "Q\n";

      QPDFObjectHandle contents =
          QPDFObjectHandle::newStream(&outpdf, content.str());
      newPage.replaceKey("/Contents", contents);

      QPDFObjectHandle newPageObj = outpdf.makeIndirectObject(newPage);
      outDh.addPage(newPageObj, false);
    }

    QPDFWriter writer(outpdf, outputPdfPath.toUtf8().constData());
    writer.write();

    return "";
  } catch (std::exception& e) {
    return e.what();
  }
}
