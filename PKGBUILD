# Maintainer: Bubu-Droid <bubu@bubudroid.me>
pkgname=isophine
pkgver=0.1.0
pkgrel=1
pkgdesc="A blazing fast PDF imposition GUI tool written in C++ using Qt and QPDF"
arch=('x86_64' 'aarch64')
url="https://github.com/Bubu-Droid/isophine"
license=('GPL-3.0-or-later')
depends=('qt6-base' 'qpdf')
makedepends=('qt6-tools' 'cmake')
source=("$pkgname-$pkgver.tar.gz::https://github.com/Bubu-Droid/isophine/archive/refs/tags/v$pkgver.tar.gz")
sha256sums=('2634cb0a40831eed942949e64b46d75a966107c567d8a4cbd60b0c6e5df81694')

build() {
  cd "$pkgname-$pkgver"
  cmake -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release
  cmake --build build
}

package() {
  cd "$pkgname-$pkgver"
  DESTDIR="$pkgdir" cmake --install build
}
