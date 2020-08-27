/**
 * @fileoverview Namespace defintions for the Protocol Buffer defintions.
 *
 * These are required because the original Protocol Buffer defintions are all
 * contained within "com.amazon.whitewater.auxproxy.pbuffer" package. The
 * Protocol Buffer definitions in this package have to emulate that structure
 * in order to be understood by the GameLift service since the socket.io events
 * are based on the full message name.
 *
 * @internal
 */

import { Namespace } from "protobufjs/light";

/**
 * Base namespace for the protocol buffer defintiions.
 *
 * @internal
 */
export const comNamespace = Namespace.fromJSON("com", {
  nested: {
    amazon: {
      nested: {
        whitewater: {
          nested: {
            auxproxy: {
              nested: {
                pbuffer: {},
              },
            },
          },
        },
      },
    },
  },
});

/**
 * Direct parent of every protocol buffer object
 *
 * @internal
 */
export const pbufferNamespace = comNamespace.lookup(
  "amazon.whitewater.auxproxy.pbuffer"
) as Namespace;
