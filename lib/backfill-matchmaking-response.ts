import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Response from the GameLift service for a
 * [`StartMatchBackfill`](https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-startmatchbackfill)
 * request.
 *
 */
class BackfillMatchmakingResponse extends Message<BackfillMatchmakingResponse> {
  /**
   * Identifier for the backfill matchmaking request ticket.
   *
   * This is mainly used in a corresponding
   * [`StopMatchBackfill`](https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-stopmatchbackfill)
   * request.
   */
  @Field.d(1, "string")
  public ticketId: string;
}

pbufferNamespace.add(BackfillMatchmakingResponse.$type);

export { BackfillMatchmakingResponse };
