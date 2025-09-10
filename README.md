# metar-cli

A command line tool to fetch and display METAR data from the requested airport.
To install or uninstall, see Installation below.

## Use

```
metar <airport_id> [airport_id...] [--translate]
```

### Plain

```
metar kfdk kdmw khgr
```

**Returns**

```
METAR KFDK 090400Z AUTO 00000KT 11/10 A3029 RMK AO2
METAR KDMW 090355Z AUTO 06003KT 10SM CLR 14/09 A3029 RMK AO2
METAR KHGR 090353Z AUTO 08004KT 10SM CLR 16/07 A3028 RMK AO2 SLP263 T01560067 $
```

### Translate


```
metar kfdk kdmw khgr
```

**Returns**

```
METAR KFDK 092147Z 08007KT 10SM CLR 24/10 A3021
METAR KDMW 092145Z AUTO 10007KT 10SM CLR 25/09 A3023 RMK AO2
METAR KHGR 092053Z 11007G14KT 10SM CLR 25/06 A3022 RMK AO2 SLP240 T02500061 56017 $

Report for KFDK:
	At 2025-09-09 22:47Z
	Wind from 80 at 7 knots
	Visibility 10 SM
	Clear below 12,000 AGL
	Temperature 24C
	Dewpoint 10C
	Pressure 30.21 inHg

Report for KDMW:
	At 2025-09-09 22:45Z
	Wind from 100 at 7 knots
	Visibility 10 SM
	Clear below 12,000 AGL
	Temperature 25C
	Dewpoint 9C
	Pressure 30.23 inHg

Report for KHGR:
	At 2025-09-09 21:53Z
	Wind from 110 at 7 gusting 14 knots
	Visibility 10 SM
	Clear below 12,000 AGL
	Temperature 25C
	Dewpoint 6C
	Pressure 30.22 inHg
```

## Installation

### From Download

- Download the latest executable for your operating system from the [Releases](https://github.com/misterrodg/metar-cli/releases) page.

**Linux and macOS**
- To install, rename the executable to `metar`, and place it in `/usr/local/bin`.
- To uninstall, remove the executable from `/usr/local/bin`.

**Windows**
- To install, rename the executable to `metar`, place it you preferred location, and [add it to your PATH](https://stackoverflow.com/questions/4822400/register-an-exe-so-you-can-run-it-from-any-command-line-in-windows).
- To uninstall, remove the PATH variable, and delete the executable from its location.

### From Source

- To install, run `./install.sh`.
- To uninstall, `./uninstall.sh`.

Building from source requires:

- Linux or macOS
- A C++ compiler
- `make`
- `cmake`
- `curl`

