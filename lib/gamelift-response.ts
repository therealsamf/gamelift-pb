import { Enum, Field, Message } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Status enum for GameLift service responses.
 *
 * @internal
 */
export enum Status {
  OK = 0,
  ERROR_400 = 1,
  ERROR_500 = 2,
}

/** @hidden */
const StatusEnum = new Enum("Enum0", {
  OK: 0,
  ERROR_400: 1,
  ERROR_500: 2,
});

/**
 * Generic response from the GameLift service. Used to determine if an SDK
 * method was in error.
 *
 * @internal
 */
class GameLiftResponse extends Message<GameLiftResponse> {
  /**
   * Status code of the SDK request.
   *
   * @internal
   */
  @Field.d(1, Status)
  public status: Status;

  /**
   * Response data as JSON, which can be parsed into the appropriate response.
   *
   * @internal
   */
  @Field.d(2, "string")
  public responseData: string;

  /**
   * Message for the response. Usually used to describe an error if one
   * occurred.
   *
   * @internal
   */
  @Field.d(3, "string")
  public errorMessage: string;
}

GameLiftResponse.$type.add(StatusEnum);
pbufferNamespace.add(GameLiftResponse.$type);

export { GameLiftResponse };
