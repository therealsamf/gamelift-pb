gamelift-pb
===========

[Protocol Buffer] messages used in the [AWS GameLift] Server SDK usable in a NodeJS environment.

This library enables communication between a NodeJS process and the AWS GameLift service.

# Getting Started

These message definitions aren't really meant to be used directly. Head over to [`@kontest/gamelift`](https://github.com/therealsamf/gamelift) for a guide and examples on how to get started with a NodeJS server running on AWS GameLift.

# Development

This library is essentially Protocol Buffer definitions implemented in [TypeScript] & [protobuf.js] reflection capability.

## Motivation

The [AWS GameLift Server SDK](https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-supported.html) doesn't include the original *.proto* files which define the Protocol Buffer messages used for communication between a dedicated game server and the GameLift service. In order to utilize the messages from a NodeJS environment there are a few options:

 1. Create a [NodeJS C++ addon](https://nodejs.org/api/addons.html) with some "glue" using the [N-API](https://nodejs.org/api/addons.html#addons_n_api) to traverse the C++/NodeJS language barrier.
  * There's other ways of implementing the specifics of this; N-API isn't specifically required. But all such solutions would require native "glue" code between NodeJS and Protocol Buffer messages compiled by `protoc` so that they're usable from JavaScript.
 2. Recreate the message definitions in a way that NodeJS can use without touching native code.
  * This could writing out the *.proto* files themselves and using `protoc` to generate NodeJS code, defining them with protobuf.js's reflection capability, or any other method for re-creating the messages in pure JavaScript.

I chose (2) over (1) for the following reasons:

 * Easier to deploy: When you start deploying native code you have to build for multiple platforms.
 * Inline documentation & type safety: Using TypeScript is advantageous since I can run [TypeDoc](https://typedoc.org/) over the files and easily generate documentation. I'm also able to utilize the static type checking with TypeScript.

    * I could still write native code while getting the benefits of TypeScript with a [declaration file](https://www.typescriptlang.org/docs/handbook/declaration-files/introduction.html). However, writing TypeScript in the first place reduces the amount of code to write.
 * Easier to extend: AWS GameLift SDK includes some model definitions which provides some helper methods on top of the fields already present in the Protocol Buffer messages. Using [protobuf.js decorator's capability](https://github.com/protobufjs/protobuf.js#using-decorators) the classes in this library fully implement GameLift's message defintiions while still being as extendable as any class in JavaScript.

## Contributing

The following guide describes how to get setup for testing and working on gamelift-pb.

**Requirements**:

In order to successfully fix bugs or add new features to this library you'll need to be able to run the test suite. Since the only reference for these Protocol Buffer messages are the original C++ class definitions generated by `protoc` the test functions & assertions are mainly written in C++. This means you'll need a toolchain capable of building native code for your development environment.

The build tool this project interfaces with directly is [`cmake-js`](https://github.com/cmake-js/cmake-js). You'll also need to install [CMake](https://cmake.org/) itself. *This project has only been tested with CMake version 3.16*. I'm sure that other versions will work, but they are not tested.

You'll also need the C/C++ compiler for your platform. See the [`cmake-js` project's README.md](https://github.com/cmake-js/cmake-js/blob/master/README.md) for some links on where to acquire those tools.

The rest of the development dependencies can be conveniently installed like any other NodeJS project:

```terminal
$ npm install
```

Or with [`yarn`](https://yarnpkg.com/)

```terminal
$ yarn install
```

### Testing

This project utilizes [Mocha.js](https://mochajs.org/) as its test runner. Simply run `npm test`.

This will run the `pretest` script first which should build the C++ addon used in the tests with `cmake-js`. `mocha` will then run the test suite defined in TypeScript files found in *tests* directory.

### Linting

This project utilizes [`eslint`](https://eslint.org/) for code quality. The script you'll want to run to lint your changes is `npm run lint`.

### Formatting

[`prettier`](https://prettier.io/) is used for formatting TypeScript and [`clang-format`](https://clang.llvm.org/docs/ClangFormat.html) can be used for formatting the C++ code found in the tests. You can use the tools respectively with `npm run format` and `npm run format:test`.

### Generating documentation

To generate the user-visible documentation run `npm run docs`. The generated docs are very stripped down and only include the messages that should outside of the GameLift SDK in most scenarios.

To generate more developer-friendly documenation you can run `npm run docs:internal`. The previously mentioned stripped internal members are included in the output.

[Node C++ addon]: https://nodejs.org/api/addons.html
[Protocol Buffer]: https://developers.google.com/protocol-buffers
[AWS GameLift]: https://aws.amazon.com/gamelift/
[TypeScript]: https://www.typescriptlang.org/
[protobuf.js]: https://github.com/protobufjs/protobuf.js
