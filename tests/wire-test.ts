/**
 * @fileoverview Test suite for running each message through the wire-test C++ addon.
 *
 * This addon checks every possible combination of every field and verifies
 * that the resulting bytes are the same as the compiled C++ Protocol Buffer
 * definitions.
 */

// @ts-ignore
const bindings = require("bindings");
import * as faker from "faker";

import * as gamelift from "../lib";

const wireTest = bindings("wire-test.node");

describe("wire-test", function () {
  Object.entries(gamelift).forEach(([key, value]) => {
    describe(key, function () {
      it("should serialize to the same result as native Protocol Buffer object", function () {
        wireTest(faker, value);
      });
    });
  });
});
