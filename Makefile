

all: development

production:
	@rm -rf dist
	@mkdir -p dist
	@cd dist && cmake -DCMAKE_BUILD_TYPE=Release .. && make

development:
	@mkdir -p dev
	@cd dev && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=../resources .. && make
	
clean:
	@rm -rf dist
	@rm -rf dev

run:
	dev/tw

flatpak:
	@flatpak-builder flatpak org.meemus.tw.json --force-clean

flatpak-run:
	@flatpak-builder --run flatpak org.meemus.tw.json tw

flatpak-makst:
	@flatpak-builder --repo=flatpak-dist --force-clean flatpak org.meemus.tw.json