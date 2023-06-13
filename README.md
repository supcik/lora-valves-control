# LoRa valves control system

Copyright (c) 2023 HEIA-FR / ISC<br/>
Haute école d'ingénierie et d'architecture de Fribourg<br/>
Informatique et Systèmes de Communication<br/>

## TTN uplink payload formatter

```javascript
function decodeUplink(input) {
  const vbat = input.bytes[0] + input.bytes[1] * 256;
  var result = {
    voltage: vbat / 100,
    valves: [0,0,0,0,0,0],
  };

  for (var i = 0; i < 6; i++) {
    result.valves[i] = (input.bytes[2] & (1 << i)) !== 0 ? 1 : 0;
  }

  return {
    data: result,
    warnings: [],
    errors: []
  };

}
```

## TTN downlink payload formatter

```javascript
const periods = [0, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, -2, -1];

function encodeDownlink(input) {
  return {
    bytes: [],
    fPort: 1,
    warnings: [],
    errors: []
  };
}

function decodeDownlink(input) {
  var result = {
    valves : [0,0,0,0,0,0]
  };

  for (var i = 0; i < 6; i++) {
    var p = input.bytes[i >> 1];
    if (i % 2 === 0) {
      p = p >> 4;
    }
    result.valves[i] = periods[p & 0xf];
  }

  return {
    data: result,
    warnings: [],
    errors: []
  }
}
```
