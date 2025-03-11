### Steps to compile and load device Tree Overlay.

`dtc -@ -I dts -o dtb -o <name-output-overlay-bin-file>.dtbo <name-input-overlay-src-file>.dts`
`sudo dtoverlay <name-output-overlay-bin-file>.dtbo`