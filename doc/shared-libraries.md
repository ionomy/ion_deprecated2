Shared Libraries
================

## ionconsensus

The purpose of this library is to make the verification functionality that is critical to Ion's consensus available to other applications, e.g. to language bindings.

### API

The interface is defined in the C header `ionconsensus.h` located in  `src/script/ionconsensus.h`.

#### Version

`ionconsensus_version` returns an `unsigned int` with the API version *(currently at an experimental `0`)*.

#### Script Validation

`ionconsensus_verify_script` returns an `int` with the status of the verification. It will be `1` if the input script correctly spends the previous output `scriptPubKey`.

##### Parameters
- `const unsigned char *scriptPubKey` - The previous output script that encumbers spending.
- `unsigned int scriptPubKeyLen` - The number of bytes for the `scriptPubKey`.
- `const unsigned char *txTo` - The transaction with the input that is spending the previous output.
- `unsigned int txToLen` - The number of bytes for the `txTo`.
- `unsigned int nIn` - The index of the input in `txTo` that spends the `scriptPubKey`.
- `unsigned int flags` - The script validation flags *(see below)*.
- `ionconsensus_error* err` - Will have the error/success code for the operation *(see below)*.

##### Script Flags
- `ionconsensus_SCRIPT_FLAGS_VERIFY_NONE`
- `ionconsensus_SCRIPT_FLAGS_VERIFY_P2SH` - Evaluate P2SH ([BIP16](https://github.com/ion/bips/blob/master/bip-0016.mediawiki)) subscripts
- `ionconsensus_SCRIPT_FLAGS_VERIFY_DERSIG` - Enforce strict DER ([BIP66](https://github.com/ion/bips/blob/master/bip-0066.mediawiki)) compliance
- `ionconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY` - Enforce NULLDUMMY ([BIP147](https://github.com/ion/bips/blob/master/bip-0147.mediawiki))
- `ionconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY` - Enable CHECKLOCKTIMEVERIFY ([BIP65](https://github.com/ion/bips/blob/master/bip-0065.mediawiki))
- `ionconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY` - Enable CHECKSEQUENCEVERIFY ([BIP112](https://github.com/ion/bips/blob/master/bip-0112.mediawiki))
- `ionconsensus_SCRIPT_FLAGS_VERIFY_WITNESS` - Enable WITNESS ([BIP141](https://github.com/ion/bips/blob/master/bip-0141.mediawiki))

##### Errors
- `ionconsensus_ERR_OK` - No errors with input parameters *(see the return value of `ionconsensus_verify_script` for the verification status)*
- `ionconsensus_ERR_TX_INDEX` - An invalid index for `txTo`
- `ionconsensus_ERR_TX_SIZE_MISMATCH` - `txToLen` did not match with the size of `txTo`
- `ionconsensus_ERR_DESERIALIZE` - An error deserializing `txTo`
- `ionconsensus_ERR_AMOUNT_REQUIRED` - Input amount is required if WITNESS is used

### Example Implementations
- [NIon](https://github.com/NicolasDorier/NIon/blob/master/NIon/Script.cs#L814) (.NET Bindings)
- [node-libionconsensus](https://github.com/bitpay/node-libionconsensus) (Node.js Bindings)
- [java-libionconsensus](https://github.com/dexX7/java-libionconsensus) (Java Bindings)
- [ionconsensus-php](https://github.com/Bit-Wasp/ionconsensus-php) (PHP Bindings)
