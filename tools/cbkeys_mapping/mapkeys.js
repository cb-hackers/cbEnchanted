#!/usr/bin/env node

var str = '',         // The information to be output to out.txt
  fs = require('fs'), // The filesystem module, we do like to write to a file, don't we?
  arr = [],           // An array that will be used to map keycodes to allegro constants
  cbStr = '',         // A string to be output to keys_allegro.cb
  // Temporary variables follow
  keyname, tmpVal, i;


// This object contains the names of allegro constants that reflect the CB SCAN-codes
var cbKeys = {
  ALLEGRO_KEY_ESCAPE: 1,
  ALLEGRO_KEY_F1: 59,
  ALLEGRO_KEY_F2: 60,
  ALLEGRO_KEY_F3: 61,
  ALLEGRO_KEY_F4: 62,
  ALLEGRO_KEY_F5: 63,
  ALLEGRO_KEY_F6: 64,
  ALLEGRO_KEY_F7: 65,
  ALLEGRO_KEY_F8: 66,
  ALLEGRO_KEY_F9: 67,
  ALLEGRO_KEY_F10: 68,
  ALLEGRO_KEY_F11: 87,
  ALLEGRO_KEY_F12: 88,
  ALLEGRO_KEY_PRINTSCREEN: 183,
  ALLEGRO_KEY_SCROLLLOCK: 70,
  ALLEGRO_KEY_NUMLOCK: 197,
  ALLEGRO_KEY_BACKSLASH2: 86,
  ALLEGRO_KEY_1: 2,
  ALLEGRO_KEY_2: 3,
  ALLEGRO_KEY_3: 4,
  ALLEGRO_KEY_4: 5,
  ALLEGRO_KEY_5: 6,
  ALLEGRO_KEY_6: 7,
  ALLEGRO_KEY_7: 8,
  ALLEGRO_KEY_8: 9,
  ALLEGRO_KEY_9: 10,
  ALLEGRO_KEY_0: 11,
  ALLEGRO_KEY_EQUALS: 12,
  ALLEGRO_KEY_OPENBRACE: 13,
  ALLEGRO_KEY_BACKSPACE: 14,
  ALLEGRO_KEY_INSERT: 210,
  ALLEGRO_KEY_HOME: 199,
  ALLEGRO_KEY_PGUP: 201,
  ALLEGRO_KEY_PAUSE: 69,
  ALLEGRO_KEY_PAD_SLASH: 181,
  ALLEGRO_KEY_PAD_ASTERISK: 55,
  ALLEGRO_KEY_PAD_MINUS: 74,
  ALLEGRO_KEY_TAB: 15,
  ALLEGRO_KEY_Q: 16,
  ALLEGRO_KEY_W: 17,
  ALLEGRO_KEY_E: 18,
  ALLEGRO_KEY_R: 19,
  ALLEGRO_KEY_T: 20,
  ALLEGRO_KEY_Y: 21,
  ALLEGRO_KEY_U: 22,
  ALLEGRO_KEY_I: 23,
  ALLEGRO_KEY_O: 24,
  ALLEGRO_KEY_P: 25,
  ALLEGRO_KEY_CLOSEBRACE: 26,
  ALLEGRO_KEY_SEMICOLON: 27,
  ALLEGRO_KEY_ENTER: 28,
  ALLEGRO_KEY_DELETE: 211,
  ALLEGRO_KEY_END: 207,
  ALLEGRO_KEY_PGDN: 209,
  ALLEGRO_KEY_PAD_7: 71,
  ALLEGRO_KEY_PAD_8: 72,
  ALLEGRO_KEY_PAD_9: 73,
  ALLEGRO_KEY_PAD_PLUS: 78,
  ALLEGRO_KEY_CAPSLOCK: 58,
  ALLEGRO_KEY_A: 30,
  ALLEGRO_KEY_S: 31,
  ALLEGRO_KEY_D: 32,
  ALLEGRO_KEY_F: 33,
  ALLEGRO_KEY_G: 34,
  ALLEGRO_KEY_H: 35,
  ALLEGRO_KEY_J: 36,
  ALLEGRO_KEY_K: 37,
  ALLEGRO_KEY_L: 38,
  ALLEGRO_KEY_TILDE: 39,
  ALLEGRO_KEY_QUOTE: 40,
  ALLEGRO_KEY_BACKSLASH: 41,
  ALLEGRO_KEY_PAD_4: 75,
  ALLEGRO_KEY_PAD_5: 76,
  ALLEGRO_KEY_PAD_6: 77,
  ALLEGRO_KEY_LSHIFT: 42,
  ALLEGRO_KEY_SLASH: 43,
  ALLEGRO_KEY_Z: 44,
  ALLEGRO_KEY_X: 45,
  ALLEGRO_KEY_C: 46,
  ALLEGRO_KEY_V: 47,
  ALLEGRO_KEY_B: 48,
  ALLEGRO_KEY_N: 49,
  ALLEGRO_KEY_M: 50,
  ALLEGRO_KEY_COMMA: 51,
  ALLEGRO_KEY_FULLSTOP: 52,
  ALLEGRO_KEY_MINUS: 53,
  ALLEGRO_KEY_RSHIFT: 54,
  ALLEGRO_KEY_PAD_1: 79,
  ALLEGRO_KEY_PAD_2: 80,
  ALLEGRO_KEY_PAD_3: 81,
  ALLEGRO_KEY_PAD_ENTER: 156,
  ALLEGRO_KEY_LCTRL: 29,
  ALLEGRO_KEY_LWIN: 219,
  ALLEGRO_KEY_ALT: 56,
  ALLEGRO_KEY_SPACE: 57,
  ALLEGRO_KEY_ALTGR: 184,
  ALLEGRO_KEY_RWIN: 220,
  ALLEGRO_KEY_MENU: 221,
  ALLEGRO_KEY_RCTRL: 157,
  ALLEGRO_KEY_PAD_0: 82,
  ALLEGRO_KEY_PAD_DELETE: 83,
  ALLEGRO_KEY_UP: 200,
  ALLEGRO_KEY_DOWN: 208,
  ALLEGRO_KEY_LEFT: 203,
  ALLEGRO_KEY_RIGHT: 205
};

// This object contains the names of all allegro keyboard constants, initialized to null
var alKeys = {
  ALLEGRO_KEY_ESCAPE: null,
  ALLEGRO_KEY_TILDE: null,
  ALLEGRO_KEY_MINUS: null,
  ALLEGRO_KEY_EQUALS: null,
  ALLEGRO_KEY_BACKSPACE: null,
  ALLEGRO_KEY_TAB: null,
  ALLEGRO_KEY_OPENBRACE: null,
  ALLEGRO_KEY_CLOSEBRACE: null,
  ALLEGRO_KEY_ENTER: null,
  ALLEGRO_KEY_SEMICOLON: null,
  ALLEGRO_KEY_QUOTE: null,
  ALLEGRO_KEY_BACKSLASH: null,
  ALLEGRO_KEY_BACKSLASH2: null,
  ALLEGRO_KEY_COMMA: null,
  ALLEGRO_KEY_FULLSTOP: null,
  ALLEGRO_KEY_SLASH: null,
  ALLEGRO_KEY_SPACE: null,
  ALLEGRO_KEY_INSERT: null,
  ALLEGRO_KEY_DELETE: null,
  ALLEGRO_KEY_HOME: null,
  ALLEGRO_KEY_END: null,
  ALLEGRO_KEY_PGUP: null,
  ALLEGRO_KEY_PGDN: null,
  ALLEGRO_KEY_LEFT: null,
  ALLEGRO_KEY_RIGHT: null,
  ALLEGRO_KEY_UP: null,
  ALLEGRO_KEY_DOWN: null,
  ALLEGRO_KEY_PAD_SLASH: null,
  ALLEGRO_KEY_PAD_ASTERISK: null,
  ALLEGRO_KEY_PAD_MINUS: null,
  ALLEGRO_KEY_PAD_PLUS: null,
  ALLEGRO_KEY_PAD_DELETE: null,
  ALLEGRO_KEY_PAD_ENTER: null,
  ALLEGRO_KEY_PRINTSCREEN: null,
  ALLEGRO_KEY_PAUSE: null,
  ALLEGRO_KEY_ABNT_C1: null,
  ALLEGRO_KEY_YEN: null,
  ALLEGRO_KEY_KANA: null,
  ALLEGRO_KEY_CONVERT: null,
  ALLEGRO_KEY_NOCONVERT: null,
  ALLEGRO_KEY_AT: null,
  ALLEGRO_KEY_CIRCUMFLEX: null,
  ALLEGRO_KEY_COLON2: null,
  ALLEGRO_KEY_KANJI: null,
  ALLEGRO_KEY_LSHIFT: null,
  ALLEGRO_KEY_RSHIFT: null,
  ALLEGRO_KEY_LCTRL: null,
  ALLEGRO_KEY_RCTRL: null,
  ALLEGRO_KEY_ALT: null,
  ALLEGRO_KEY_ALTGR: null,
  ALLEGRO_KEY_LWIN: null,
  ALLEGRO_KEY_RWIN: null,
  ALLEGRO_KEY_MENU: null,
  ALLEGRO_KEY_SCROLLLOCK: null,
  ALLEGRO_KEY_NUMLOCK: null,
  ALLEGRO_KEY_CAPSLOCK: null,
  ALLEGRO_KEY_PAD_EQUALS: null,
  ALLEGRO_KEY_BACKQUOTE: null,
  ALLEGRO_KEY_SEMICOLON2: null,
  ALLEGRO_KEY_COMMAND: null,
  ALLEGRO_KEY_A: null,
  ALLEGRO_KEY_B: null,
  ALLEGRO_KEY_C: null,
  ALLEGRO_KEY_D: null,
  ALLEGRO_KEY_E: null,
  ALLEGRO_KEY_F: null,
  ALLEGRO_KEY_G: null,
  ALLEGRO_KEY_H: null,
  ALLEGRO_KEY_I: null,
  ALLEGRO_KEY_J: null,
  ALLEGRO_KEY_K: null,
  ALLEGRO_KEY_L: null,
  ALLEGRO_KEY_M: null,
  ALLEGRO_KEY_N: null,
  ALLEGRO_KEY_O: null,
  ALLEGRO_KEY_P: null,
  ALLEGRO_KEY_Q: null,
  ALLEGRO_KEY_R: null,
  ALLEGRO_KEY_S: null,
  ALLEGRO_KEY_T: null,
  ALLEGRO_KEY_U: null,
  ALLEGRO_KEY_V: null,
  ALLEGRO_KEY_W: null,
  ALLEGRO_KEY_X: null,
  ALLEGRO_KEY_Y: null,
  ALLEGRO_KEY_Z: null,

  ALLEGRO_KEY_0: null,
  ALLEGRO_KEY_1: null,
  ALLEGRO_KEY_2: null,
  ALLEGRO_KEY_3: null,
  ALLEGRO_KEY_4: null,
  ALLEGRO_KEY_5: null,
  ALLEGRO_KEY_6: null,
  ALLEGRO_KEY_7: null,
  ALLEGRO_KEY_8: null,
  ALLEGRO_KEY_9: null,

  ALLEGRO_KEY_PAD_0: null,
  ALLEGRO_KEY_PAD_1: null,
  ALLEGRO_KEY_PAD_2: null,
  ALLEGRO_KEY_PAD_3: null,
  ALLEGRO_KEY_PAD_4: null,
  ALLEGRO_KEY_PAD_5: null,
  ALLEGRO_KEY_PAD_6: null,
  ALLEGRO_KEY_PAD_7: null,
  ALLEGRO_KEY_PAD_8: null,
  ALLEGRO_KEY_PAD_9: null,

  ALLEGRO_KEY_F1: null,
  ALLEGRO_KEY_F2: null,
  ALLEGRO_KEY_F3: null,
  ALLEGRO_KEY_F4: null,
  ALLEGRO_KEY_F5: null,
  ALLEGRO_KEY_F6: null,
  ALLEGRO_KEY_F7: null,
  ALLEGRO_KEY_F8: null,
  ALLEGRO_KEY_F9: null,
  ALLEGRO_KEY_F10: null,
  ALLEGRO_KEY_F11: null,
  ALLEGRO_KEY_F12: null
}

// Let's go through every valid value in allegro constants (the ones that are null)
// and see if we have a matching key in the allegro-key -> coolbasic-constant object
for (keyname in alKeys) {
  var cbKey = cbKeys[keyname];
  if (cbKey) {
    //str += 'KEYS[' + cbKey + '] = ' + keyname + ';\n';
    alKeys[keyname] = cbKey;
    cbKeys[keyname] = null;
    arr[cbKey] = keyname;
    delete cbKeys[keyname];
  }
}

// Let's list every CB constant that we missed, converting it to the cbKey<name> format first
str += 'No match found for the following CoolBasic constants:\n---\n'
for (keyname in cbKeys) {
  tmpVal = cbKeys[keyname];
  if (tmpVal !== null) {
    keyname = keyname.replace('ALLEGRO_KEY_', '');
    keyname = 'cbKey' + keyname.charAt(0) + keyname.substring(1).toLowerCase();
    str += keyname + ' = ' + tmpVal + '\n';
  }
  delete alKeys[keyname];
}
str += '---\n';

// Let's list every Allegro constant that got left without a pair
str += '\nNo match found for the following Allegro constants:\n---\n';
for (keyname in alKeys) {
  if (alKeys[keyname] === null) {
    str += keyname + '\n';
  }
}
str += '---\n';

// Let's create a nice-looking array-ready code of every value that we found
// It will be listed as KEYMAP[int keycode] = <allegro_constant>
str += '\nHere\'s your pretty array, young boy:\n--- --- ---\n';
for (i = 0; i < arr.length; i++) {
  if (arr[i]) {
    str += 'KEYMAP[' + i + '] = ' + arr[i] + '\n';
  }
}
str += '--- --- ---';

// Write what we found out to out.txt
console.log('\nWriting to file out.txt');
fs.writeFile('out.txt', str, function (err) {
  if (err) throw err;
  console.log('Writing to out.txt complete');
});

// Create a nice include-ready piece of code for CoolBasic to use
cbStr += 'Dim allegro(' + arr.length + ') As String\r\n';
cbStr += 'For i = 0 To ' + arr.length + '\r\n';
cbStr += '    allegro(i) = "-none-"\r\n';
cbStr += 'Next i\r\n';
cbStr += '\r\n';

for (i = 0; i < arr.length; i++) {
  if (arr[i]) {
    cbStr += 'allegro(' + i + ') = "' + arr[i] + '"\r\n';
  }
}

// Write the CB-code to keys_allegro.cb file
console.log('\nWriting to keys_allegro.cb');
fs.writeFile('keys_allegro.cb', cbStr, function (err) {
  if (err) throw err;
  console.log('Writing to keys_allegro.cb complete');
});