#pragma once

#include "project_settings.h"

std::string PDFRenderer(
    const QString& inputPdfPath,
    const QString& outputPdfPath,
    const std::vector<PageTransform>& pageTransformVector,
    const QColor& bgColor,
    const QSizeF& pageDimensions
);
