# metar-cli

A command line tool to fetch and display METAR data from the requested airport.
To install or uninstall, see Installation below.

## Use

```bash
metar <airport_id> [airport_id...] [-t | --translate] [-m="METAR STRING" | --metar="METAR STRING"]
```

The command `metar`, followed by at least one `airport_id` will result in a [basic](#basic) output.
An optional `--translate` (or `-t`) flag for a more detailed [translated](#translated) output.
An optional `--metar` (of `-m`) flag to manually parse a given METAR string.

Inputs are not case sensitive: `METAR KIAD` or `metar kiad` are handled the same.

### Basic

```bash
metar kfdk kdmw khgr
```

**Returns**

```
METAR KFDK 090400Z AUTO 00000KT 11/10 A3029 RMK AO2
METAR KDMW 090355Z AUTO 06003KT 10SM CLR 14/09 A3029 RMK AO2
METAR KHGR 090353Z AUTO 08004KT 10SM CLR 16/07 A3028 RMK AO2 SLP263 T01560067 $
```

### Translated

```bash
# You can use the --translate or -t flag
metar kfdk kdmw khgr --translate
```

**Returns**

```
METAR KDMW 222015Z AUTO 13006KT 7SM RA BKN008 OVC019 14/14 A3030 RMK AO2 P0005
METAR KHGR 221953Z 08005KT 6SM -RA BR OVC018 14/13 A3029 RMK AO2 SLP267 P0006 T01440133 $
METAR KFDK 221947Z 10005KT 5SM -RA BR FEW012 BKN042 OVC048 14/14 A3032

METAR Report for KDMW:
	On the 22nd at 20:15Z
	Automated
	Wind from 130 at 6 knots
	Visibility 7 statute miles
	Weather:
		Rain
	Cloud cover:
		OVC at 1900 AGL
		BKN at 800 AGL --ceiling--
	Temperature 14C
	Dewpoint 14C
	Pressure 30.30 inHg

METAR Report for KHGR:
	On the 22nd at 19:53Z
	Wind from 80 at 5 knots
	Visibility 6 statute miles
	Weather:
		Light Rain
		Mist
	Cloud cover:
		OVC at 1800 AGL --ceiling--
	Temperature 14C
	Dewpoint 13C
	Pressure 30.29 inHg

METAR Report for KFDK:
	On the 22nd at 19:47Z
	Wind from 100 at 5 knots
	Visibility 5 statute miles
	Weather:
		Light Rain
		Mist
	Cloud cover:
		OVC at 4800 AGL
		BKN at 4200 AGL --ceiling--
		FEW at 1200 AGL
	Temperature 14C
	Dewpoint 14C
	Pressure 30.32 inHg
```

## Installation

Installation can be by [Download](#download), or by
[Building From Source](#building-from-source).

### Download

- Download the latest executable for your operating system from the [Releases](https://github.com/misterrodg/metar-cli/releases) page.

**Linux and macOS**

Installation and removal is simple with the `./install.sh` and `./uninstall.sh`
scripts available in the project root.

Install

1. Rename the executable to `metar`.
2. Make it executable: `chmod +x metar`.
3. Move it to your path: `sudo mv metar /usr/local/bin`.

Uninstall

1. Remove the executable from `/usr/local/bin`.

**Windows**

Install

1. Rename the executable to `metar`.
2. Place it in the location of your choice.
3. [Add it to your PATH](https://stackoverflow.com/questions/4822400/register-an-exe-so-you-can-run-it-from-any-command-line-in-windows).

Uninstall

1. Remove the PATH variable, and delete the executable from its location.

### Building From Source

Requirements:

- Linux or macOS
- A C++ compiler
- `make`
- `cmake`
- `curl`

To build, run `./build.sh`.
