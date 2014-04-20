# Maintainer: Prologin <serveur@prologin.org>
# Contributor: Prologin <serveur@prologin.org>

pkgname=stechec2-prologin2014
pkgver=2014_0.1
pkgrel=1
pkgdesc="Stechec2 with Prologin 2014 rules"
arch=('i686' 'x86_64')
url="http://prologin.org/"
license=('GPL')
provides=('stechec2' 'prologin2014')
depends=('google-gflags' 'python2' 'zeromq')
makedepends=('git')
source=("git+https://bitbucket.org/prologin/stechec2"
        "git+https://bitbucket.org/prologin/prologin2014")
sha512sums=(SKIP
            SKIP)

prepare() {
    cd stechec2
    ln --symbolic "$srcdir/prologin2014" games/
}

build() {
    cd stechec2
    python2 waf.py configure build --prefix=/usr --with-games=prologin2014
}

package() {
    cd stechec2
    python2 waf.py install --destdir="$pkgdir"
}
