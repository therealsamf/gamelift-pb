import { MapField, Message, Field } from "protobufjs/light";

import { pbufferNamespace } from "./namespace";

/**
 * Values for use in {@link Player} attribute key-value pairs.
 *
 * This object lets you specify an attribute value using any of the valid
 * data types: string, number, string array, or data map. Each AttributeValue
 * object can use only one of the available properties.
 *
 */
class AttributeValue extends Message<AttributeValue> {
  /** @hidden */
  @Field.d(1, "int32")
  public type: number;

  /**
   * For single string values.
   *
   * Maximum string length is 100 characters.
   */
  @Field.d(2, "string")
  public S: number;

  /**
   * For number values, expressed as double.
   */
  @Field.d(3, "double")
  public N: number;

  /**
   * For a list of up to 10 strings.
   *
   * Maximum length for each string is 100 characters. Duplicate values are
   * not recognized; all occurrences of the repeated value after the first of
   * a repeated value are ignored.
   */
  @Field.d(4, "string", "repeated")
  public SL: string[];

  /**
   * For a map of up to 10 data string:double pairs.
   *
   * Maximum length for each string value is 100 characters.
   */
  @MapField.d(5, "string", "double")
  public SDM: { [key: string]: number };
}

pbufferNamespace.add(AttributeValue.$type);

export { AttributeValue };
