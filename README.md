# metar-cli

A command line tool to fetch and display METAR data from the requested airport.

## Use

```
metar <airport_id> [airport_id...]
```

**Example**

```
metar kfdk kdmw khgr
```

**Returns**

```
METAR KFDK 090400Z AUTO 00000KT 11/10 A3029 RMK AO2
METAR KDMW 090355Z AUTO 06003KT 10SM CLR 14/09 A3029 RMK AO2
METAR KHGR 090353Z AUTO 08004KT 10SM CLR 16/07 A3028 RMK AO2 SLP263 T01560067 $
```

