// |reftest| skip -- BigInt is not supported
// Copyright (C) 2016 the V8 project authors. All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.
/*---
esid: sec-typedarray-object
description: >
  Return abrupt from length property as a Symbol on the object argument
info: |
  22.2.4.4 TypedArray ( object )

  This description applies only if the TypedArray function is called with at
  least one argument and the Type of the first argument is Object and that
  object does not have either a [[TypedArrayName]] or an [[ArrayBufferData]]
  internal slot.

  ...
  5. Let len be ? ToLength(? Get(arrayLike, "length")).
  ...
includes: [testBigIntTypedArray.js]
features: [BigInt, Symbol, TypedArray]
---*/

var obj = {
  length: Symbol("1")
};

testWithBigIntTypedArrayConstructors(function(TA) {
  assert.throws(TypeError, function() {
    new TA(obj);
  });
});

reportCompare(0, 0);
