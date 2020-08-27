import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

import { Player } from "./player";

/**
 * This data type is used to send a matchmaking backfill request.
 *
 * The information is communicated to the GameLift service in a
 * [`StartMatchBackfill()`] call.
 *
 * [`StartMatchBackfill()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-startmatchbackfill
 */
class BackfillMatchmakingRequest extends Message<BackfillMatchmakingRequest> {
  /**
   * Unique game session identifier.
   *
   * The API action [`GetGameSessionId()`] returns the identifier in ARN
   * format.
   *
   * [`GetGameSessionId()`]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-getgamesessionid
   */
  @Field.d(2, "string")
  public gameSessionArn: string;

  /**
   * Unique identifier, in the form of an ARN, for the matchmaker to use in
   * for this request.
   *
   * To find the matchmaker that was used to create the original game
   * session, look in the game session object, in the matchmaker data
   * property.
   *
   * Learn more about matchmaker data in [Work with Matchmaker data].
   *
   * [Work with Matchmaker data]: https://docs.aws.amazon.com/gamelift/latest/developerguide/match-server.html#match-server-data
   */
  @Field.d(3, "string")
  public matchmakingConfigurationArn: string;

  /**
   * A set of data representing all players who are currently in the
   * game session.
   *
   * The matchmaker uses this information to search for new players who are
   * good matches for the current players. See the Amazon GameLift API
   * Reference Guide for a description of the Player object format. To find
   * player attributes, IDs, and team assignments, look in the game session
   * object, in the matchmaker data property. If latency is used by the
   * matchmaker, gather updated latency for the current region and include
   * it in each player's data.
   */
  @Field.d(4, Player, "repeated")
  public players: Player[];

  /**
   * Unique identifier for a matchmaking or match backfill request ticket.
   *
   * If no value is provided here, Amazon GameLift will generate one in the
   * form of a UUID. Use this identifier to track the match backfill ticket
   * status or cancel the request if needed.
   */
  @Field.d(1, "string")
  public ticketId: string;
}

pbufferNamespace.add(BackfillMatchmakingRequest.$type);

export { BackfillMatchmakingRequest };
