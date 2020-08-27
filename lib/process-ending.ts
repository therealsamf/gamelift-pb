import { Message, Type } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Message that notifies the GameLift service that the process is ending.
 *
 * See [`ProcessEnding()`] for more information.
 *
 * [`ProcessEnding()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-processending
 *
 * @internal
 */
@Type.d("ProcessEnding")
class ProcessEnding extends Message<ProcessEnding> {}

pbufferNamespace.add(ProcessEnding.$type);

export { ProcessEnding };
