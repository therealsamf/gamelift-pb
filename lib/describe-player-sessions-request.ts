import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * This data type is used to specify which player session(s) to retrieve. You can use
 * it as follows:
 *
 *  * Provide a PlayerSessionId to request a specific player session.
 *  * Provide a GameSessionId to request all player sessions in the specified game
 * session.
 *  * Provide a PlayerId to request all player sessions for the specified player.
 *
 * For large collections of player sessions, use the pagination parameters to retrieve
 * results in sequential blocks.
 *
 */
class DescribePlayerSessionsRequest extends Message<
  DescribePlayerSessionsRequest
> {
  /**
   * Unique game session identifier.
   *
   * Use this parameter to request all player sessions for the specified
   * game session. Game session ID format is as follows:
   * `arn:aws:gamelift:<region>::gamesession/fleet-<fleet ID>/<ID string>`.
   * The value of \<ID string\> is either a custom ID string or (if one was
   * specified when the game session was created) a generated string.
   */
  @Field.d(1, "string")
  gameSessionId: string;

  /**
   * Unique identifier for a player.
   *
   * Player IDs are defined by the developer. See [Generating Player IDs].
   *
   * [Generating Player IDs]: https://docs.aws.amazon.com/gamelift/latest/developerguide/player-sessions-player-identifiers.html
   */
  @Field.d(2, "string")
  playerId: string;

  /**
   * Unique identifier for a player session.
   */
  @Field.d(3, "string")
  playerSessionId: string;

  /**
   * Player session status to filter results on.
   *
   * Possible player session statuses include the following:
   *
   *  * RESERVED – The player session request has been received, but the
   * player has not yet connected to the server process and/or been
   * validated.
   *  * ACTIVE – The player has been validated by the server process and is
   * currently connected.
   *  * COMPLETED – The player connection has been dropped.
   *  * TIMEDOUT – A player session request was received, but the player did
   * not connect and/or was not validated within the time-out limit (60 seconds).
   */
  @Field.d(4, "string")
  playerSessionStatusFilter: "RESERVED" | "ACTIVE" | "COMPLETED" | "TIMEDOUT";

  /**
   * Token indicating the start of the next sequential page of results.
   *
   * Use the token that is returned with a previous call to this action. To
   * specify the start of the result set, do not specify a value. If a player
   * session ID is specified, this parameter is ignored.
   */
  @Field.d(5, "string")
  nextToken: string;

  /**
   * Maximum number of results to return.
   *
   * Use this parameter with
   * {@link nextToken | `nextToken`} to get results as a set of sequential
   * pages. If a player session ID is specified, this parameter is ignored.
   */
  @Field.d(6, "int32")
  limit: number;
}

pbufferNamespace.add(DescribePlayerSessionsRequest.$type);

export { DescribePlayerSessionsRequest };
