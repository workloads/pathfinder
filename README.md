# Pathfinder

> ESP32-based mobile edge computing device based on the Waveshare UGV platform

## Table of Contents

<!-- TOC -->
* [Pathfinder](#pathfinder)
  * [Table of Contents](#table-of-contents)
  * [Requirements](#requirements)
    * [Hardware](#hardware)
      * [Mobility Platform](#mobility-platform)
      * [Compute Environment](#compute-environment)
      * [Camera Platform](#camera-platform)
    * [Software](#software)
      * [Development](#development)
  * [Usage](#usage)
  * [Notes](#notes)
    * [Hardware Datasheets](#hardware-datasheets)
    * [Delays](#delays)
  * [Contributors](#contributors)
  * [License](#license)
<!-- TOC -->

## Requirements

### Hardware

A _Pathfinder_ unit consists of three distinct groups of hardware components:

- Mobility Platform 
- compute environment
- camera platform

#### Mobility Platform

> The _Mobility Platform_ (`MP`) provides non-tethered movement and power-supply capabilities for the _Pathfinder_ unit. 

This group consists of:

- 1 x Waveshare [WAVE ROVER](https://www.waveshare.com/wave-rover.htm)
- 1 x TODO Antenna
- 3 x 18650 Lithium batteries (min. 3000mAh capacity)

#### Compute Environment

> The _Compute Environment_ (`CE`) provides processing and networking capabilities for the _Pathfinder_ unit.

This group consists of:

- 1 x [Raspberry Pi 5](https://www.raspberrypi.com/products/raspberry-pi-5/) (8GB memory, SKU `RP-5B-8GB`)
- 1 x [Raspberry Pi Active Cooler](https://www.raspberrypi.com/products/active-cooler/)
- 1 x [Raspberry Pi AI Kit](https://www.raspberrypi.com/products/ai-kit/)
- 1 x microSD card (minimum 64GB capacity)

#### Camera Platform

The _Camera Platform_ (`CP`) provides visual detection capabilities for the _Pathfinder_ unit.

This group consists of:

- 1 x Waveshare [2-axis Pan-Tilt Camera Module](https://www.waveshare.com/2-axis-pan-tilt-camera-module.htm)
- 1 x youyeetoo [FHL-LD19P](https://wiki.youyeetoo.com/en/Lidar/D300) Lidar device (or compatible)

### Software

- a copy of [@workloads/tooling](https://github.com/workloads/tooling)
- [Arduino IDE](https://www.arduino.cc/en/software)
- Arduino CLI `1.0.0` or [newer](https://arduino.github.io/arduino-cli/)
- Arduino Board Core for ESP32 [v2.x.x](https://github.com/espressif/arduino-esp32)
- Arduino Libraries 
  
For an exhaustive list of Core and Library versions, see [./sketch.yaml](./sketch.yaml).

#### Development

For development and testing of this repository:

* `arduino-lint` `1.2.0` or [newer](https://arduino.github.io/arduino-lint/)
* `cpplint` `1.6.0` or [newer](https://github.com/cpplint/cpplint)
* `clang-format` `18.1.0` or [newer](https://clang.llvm.org/docs/ClangFormat.html)
* ESP32 Toolchain `5.2.0` or [newer](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/index.html#installation)

## Usage

This repository provides a [Makefile](./Makefile)-based workflow.

Running `make` without commands will print out the following help information:

```text
🧭 PATHFINDER

Target           Description                                     Usage
lint-arduino     lint Arduino code using arduino-lint            `make lint-arduino`
lint-cpp         lint C++ code using cpplint and clang-format    `make lint-cpp`
lint-yaml        lint YAML files                                 `make lint-yaml`
attach           attach Arduino Board using arduino-cli          `make attach`
clean            clean Arduino cache using arduino-cli           `make clean`
compile          compile Arduino Sketch using arduino-cli        `make compile`
upload           upload binary artifact using arduino-cli        `make upload`
monitor          monitor binary output using arduino-cli         `make monitor`
install-cores    install Arduino Board Core using arduino-cli    `make install-cores`
install-libs     install Arduino libraries using arduino-cli     `make install-libs`
list-libs        list Arduino libraries using arduino-cli        `make list-libs`
docs             create docs using doxygen                       `make docs`
docs-get-theme   retrieve Doxygen theme using git                `make docs-get-theme`
help             display a list of Make Targets                  `make help`
_listincludes    list all included Makefiles and *.mk files      `make _listincludes`
_selfcheck       lint Makefile                                   `make _selfcheck`
```

## Notes

### Hardware Datasheets

This repository does not include any datasheets directly.

For an overview of the WAVE ROVER hardware, see [waveshare.com](https://www.waveshare.com/wiki/WAVE_ROVER#Resource).

For an overview of the ESP32 hardware, see [espressif.com](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf).

### Delays

All use of the [`delay()`](https://www.arduino.cc/reference/en/language/functions/time/delay/) function is intentional, despite the function's blocking behavior.

## Contributors

For a list of current (and past) contributors to this repository, see [GitHub](https://github.com/workloads/pathfinder/graphs/contributors).

## License

Licensed under the General Public License, Version 3.0 (the "License").

You may download a copy of the License at [gnu.org/licenses/gpl-3.0.txt](https://www.gnu.org/licenses/gpl-3.0.txt).

See the License for the specific language governing permissions and limitations under the License.
