import { Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

import { PlayerSession } from "./player-session";

/**
 * Response from the GameLift service for a [DescribePlayerSessionsRequest].
 *
 * [DescribePlayerSessionsRequest]: https://docs.aws.amazon.com/gamelift/latest/developerguide/integration-server-sdk-cpp-ref-actions.html#integration-server-sdk-cpp-ref-describeplayersessions
 */
class DescribePlayerSessionsResponse extends Message<
  DescribePlayerSessionsResponse
> {
  /**
   * Token that indicates where to resume retrieving results on the next
   * call to this action. If no token is returned, these results represent
   * the end of the list.
   */
  @Field.d(1, "string")
  nextToken?: string;

  /**
   * A collection of objects containing properties for each player session
   * that matches the request.
   */
  @Field.d(2, PlayerSession, "repeated")
  playerSessions: PlayerSession[];
}

pbufferNamespace.add(DescribePlayerSessionsResponse.$type);

export { DescribePlayerSessionsResponse };
