

all: development

production:
	@rm -rf dist
	@mkdir -p dist
	@cd dist && cmake .. && make

development:
	@mkdir -p dev
	@cd dev && cmake -DCMAKE_INSTALL_PREFIX=../resources .. && make
	
clean:
	@rm -rf dist
	@rm -rf dev

run:
	dev/dingus

flatpak:
	@flatpak-builder flatpak org.meemus.dingus.json --force-clean

flatpak-run:
	@flatpak-builder --run flatpak org.meemus.dingus.json dingus

flatpak-dist:
	@flatpak-builder --repo=flatpak-dist --force-clean flatpak org.meemus.dingus.json