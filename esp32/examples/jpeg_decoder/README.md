# jpeg decoder

This application demos decoding of jpeg image.

### Install the ESP IDF

Follow the instructions of the [ESP-IDF get started guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html) to setup ESP-IDF and the toolchain.

  - Recommended IDF version: `release/v4.2`

The next steps assume that the [IDF environment variables are set](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#step-4-set-up-the-environment-variables) :

 * The `IDF_PATH` environment variable is set. (`export IDF_PATH=/path/to/esp-idf/`)
 * `idf.py` and Xtensa-esp32 tools (e.g. `xtensa-esp32-elf-gcc`) are in `$PATH` (Can be done using `./install.sh` and `. ./export.sh` under `$IDF_PATH` dir on linux/mac)

### Building the example

Run following command:
```
idf.py build
```

### Load and run the example

To flash (replace `/dev/ttyUSB0` with the device serial port):
```
idf.py --port /dev/ttyUSB0 flash
```

Monitor the serial output:
```
idf.py --port /dev/ttyUSB0 monitor
```

Use `Ctrl+]` to exit.

The previous two commands can be combined:
```
idf.py --port /dev/ttyUSB0 flash monitor
```
