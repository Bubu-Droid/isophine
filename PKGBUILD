# Maintainer: Bubu-Droid <bubu@bubudroid.me>
pkgname=isophine
pkgver=0.1.1
pkgrel=1
pkgdesc="A blazing fast PDF imposition GUI tool written in C++ using Qt and QPDF"
arch=('x86_64' 'aarch64')
url="https://github.com/Bubu-Droid/isophine"
license=('GPL-3.0-or-later')
depends=('qt6-base' 'qpdf')
makedepends=('qt6-tools' 'cmake')
source=("$pkgname-$pkgver.tar.gz::https://github.com/Bubu-Droid/isophine/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('e1180114c03aee40fea6f98e654d160221b2b92b55a914823870b4f0d5d48fc3')

build() {
  cd "$pkgname-$pkgver"
  cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
  cmake --build build
}

package() {
  cd "$pkgname-$pkgver"
  DESTDIR="$pkgdir" cmake --install build
}
