===============================================================================================
1. EXAMPLES:
===============================================================================================
1.1 STEPS TO CREATE AND VERIFY/RECOVER CERT.CASD

     Example Scripts:
     CASDCertExampleScen1.jcsh, 
     CASDCertExampleScen2A.jcsh,
     
     The above example scripts show how to create a CERT.CASD.AUT/CERT.CASD.CT
     case you need to personalize your CASD, and how to verify a
     CERT.CASD.AUT/CERT.CASD.CT and recover the public CASD key.
     
     
     1.2 STEPS TO CREATE CERT.AP
     
     Example Scripts:
     APSDCertExample.jcsh
     
     This example shows how to create a certificate for an APSD.
     
     1.3 STEPS TO CREATE AND VERIFY/RECOVER A SIGNED AND ENCRYPTED RKG
     
     Example Scripts:
     gp22_AmdA_OnCard.jcsh
     
     This example creates a fake response for the OBGK PULL request of the APSD.
     With this faked response it is shown how to verify the OBGK signature,
	 how to recover the OBGK from it and how to derive the session keys from
	 the OBGK.
     
1.4 STEPS TO ENCRYPT SESSION KEYS AND VERIFY/RECOVER SIGNED AND ENCRYPTED SECURE CHANNEL KEYS
     
     Example Scripts:
     deriveSessionKeyExample.jcsh – This example implements the steps involved
                                    in Scenario 1: Pull Model using PK Scheme.
     pushSessionKeyExample.jcsh   - This example implements the steps involved in 
                                    Scenario 2A: Push Model using AP certificate 
     							    and Scenario 2B: Push Model without AP
									certificate.
     pullModelNonPKExample.jcsh   - This example implements the steps involved
                                    in Scenario 1: Pull Model Non PK Scheme.
							   
===============================================================================================
2. CRYPTO PLUGIN:
===============================================================================================

2.1  SHA-1

     SHA-1 is supported by hash command.
     Below is detailed description of command
     usage: 
     hash [[-m|--mode algorithm][data [pattern..]*

     -m|--mode algorithm : The mode of operation.
     data                : Data
     
     The following modes are supported:
     SHA     - Same as SHA-1
     SHA-1   - The Secure Hash Algorithm, as defined in Secure Hash
		       Standard, NIST FIPS 180-1
     SHA-256 - Secure hash Algorithm SHA-2 as of NIST FIPS 180-3
     SHA-384 - Secure hash Algorithm SHA-2 as of NIST FIPS 180-3
     SHA-512 - Secure hash Algorithm SHA-2 as of NIST FIPS 180-3
     MD2     - The MD2 message digest algorithm as defined in RFC 1319
     MD5     - The MD5 message digest algorithm as defined in RFC 1321
     
     This command computes a message digest over the provided data. The 'mode'
     determines the digest algorithm. It defaults to 'SHA-1' if no mode is 
     explicitely requested.

     Examples – 
     crypto>hash SHA-1 00112233445566778899AABBCCDDEEFF

2.2 RSA KEY PAIR GENERATION

     keygen’ is a command to generate RSA key pair.
     Below is detailed description of command
     usage: 
     keygen [-m|--mode algorithm] public-exponent [-l|keylength key length] 
     
     -m|--mode	algorithm    : The mode of operation. Currently ONLY RSA is
                               supported.
      public-exponent        : Public exponent for key generation          
     -l|keylength key length : Length of key in number of bits
	 
     This command calculates the key pair for RSA and sets environment
	 variables for public key and private key. Calculated keys will be stored
	 in ‘KEY_RSA_PublicKey’ and ‘KEY_RSA_PrivateKey’ environment variables. 
     
     Both of These environment variables will have modulus and exponent in LV
	 format.
     [[modulus length][modulus][exponent length][exponent]]
     
     Examples:
     crypto>kegen –m RSA 00010001 1024
	 
2.3  RSA ENCRYPTION WITH NODPAD AND PKCS#1 v1.5 PADDING
     
     RSA Encryption is done using encrypt command with mode RSA
	 usage: encrypt [[-m|--mode algorithm][-k|--key key][-p|--pad algorithm][-i|--iv vector][data [pattern..]* 

     -m|--mode algorithm : The mode of operation.
                           The following modes are supported:
                           'RSA' - Perform an encryption/decryption task in RSA
                           mode.Only NOPAD & PKCS1 will be used for RSA
                           algorithm.						   
            
     -k|--key key        : Key for encryption,for RSA key contains
                           modulus and exponent in below format. 
                           [len modulus][modulus][len exponent][exponent]
                        
     -p|--pad padding 
		  algorithm      : Defines the padding algorithm to be applied.
                           'NOPAD' - No padding will be performed
                           'PKCS1' - Padding according to PKCS#1 v1.5 std.
                           It will be used ONLY in case of RSA.
						   
     -i|--iv vector      : The initialization vector to seed the initial
                           transformation function which is applied at the
                           beginning of the MAC algorithm.
                           ** not used for RSA encryption
            
      data               : Data

	 
      Example 1: This command performs RSA encryption with no padding
      crypto>encrypt –m RSA NOPAD 80A2E2668B050F5E9928F2A0769F18823E87C6A5F9F73F9F6485A2CE46D6F095F5B93ADF07AFA1BA58A1653C9C049E5147EB96AB9FD78FA64F591FA218B5E0AF5F5917061B6C42F64E37F07B9EB1E00C36A4A60ACDF4DB54609898E724F94F293BCBFE70D90E28AE34CD3E3981B4075C4F9213464BAF7BDD38E9217ECB4EA617FF0400010001 00110011001100110011001100110011
      
      Example 2: This command performs RSA encryption with PKCS1 v1.5 standard padding
      crypto>encrypt –m RSA PKCS1 80A2E2668B050F5E9928F2A0769F18823E87C6A5F9F73F9F6485A2CE46D6F095F5B93ADF07AFA1BA58A1653C9C049E5147EB96AB9FD78FA64F591FA218B5E0AF5F5917061B6C42F64E37F07B9EB1E00C36A4A60ACDF4DB54609898E724F94F293BCBFE70D90E28AE34CD3E3981B4075C4F9213464BAF7BDD38E9217ECB4EA617FF0400010001 00110011001100110011001100110011
      
2.4  RSA DECRYPTION WITH NOPAD AND PKCS#1 v1.5 PADDING

     RSA decryption is done with decrypt command with mode as RSA
     	 
     usage:
     decrypt [-m|--mode algorithm][-p|--pad algorithm][-k|--key key][-i|--iv vector] data [pattern..] 
     -m|--mode algorithm : The mode of operation.
                           'RSA'- Perform an encryption/decryption task in RSA
                           mode.Only NOPAD and PKCS1 will be used for RSA
						   algorithm.

     -p|--pad algorithm  : Defines the padding algorithm to be applied.
                           'NOPAD' - No padding will be performed
                           'PKCS1' - Padding according to PKCS#1 v1.5 std.
                           It will be used ONLY in case of RSA 

     -k|--key key        : Key used for decryption, for RSA key contains
                           modulus and exponent in below format 
                           [len modulus][modulus][len exponent][exponent]
     
     -i|--iv vector      : The initialization vector to seed the initial
	                       transformation function which is applied at the
                           beginning of the MAC algorithm.
                           ** not used for RSA decryption
     data                : Data
	 
     Example 1:  This command performs RSA decryption with no padding.
     crypto>decrypt –m RSA NOPAD 80A2E2668B050F5E9928F2A0769F18823E87C6A5F9F73F9F6485A2CE46D6F095F5B93ADF07AFA1BA58A1653C9C049E5147EB96AB9FD78FA64F591FA218B5E0AF5F5917061B6C42F64E37F07B9EB1E00C36A4A60ACDF4DB54609898E724F94F293BCBFE70D90E28AE34CD3E3981B4075C4F9213464BAF7BDD38E9217ECB4EA617FF8098327E688618DF66B3894A80AC962515329F5E755C1F39DB9EB70A91CCC34F0C24C12AA72EE82A8C287AC5106F8C531CA5B8CAB2F7A3C55DBD443E9D624D08EFCC14D6ED292BEF95CE02E88A14C59DAD4595676CE38CEEBD1D8527DA5D00AF0198AAE9FC17A3B4E5D7901B4347948A8A6994A30FC7A68E3D4A59F18134490EF9  12F31B2C12F31B212F31B2C07301C9E9E

	 Example 2:  This command performs RSA decryption with PKCS1 v1.5
                 standard padding
	 crypto>decrypt –m RSA PKCS1 80A2E2668B050F5E9928F2A0769F18823E87C6A5F9F73F9F6485A2CE46D6F095F5B93ADF07AFA1BA58A1653C9C049E5147EB96AB9FD78FA64F591FA218B5E0AF5F5917061B6C42F64E37F07B9EB1E00C36A4A60ACDF4DB54609898E724F94F293BCBFE70D90E28AE34CD3E3981B4075C4F9213464BAF7BDD38E9217ECB4EA617FF8098327E688618DF66B3894A80AC962515329F5E755C1F39DB9EB70A91CCC34F0C24C12AA72EE82A8C287AC5106F8C531CA5B8CAB2F7A3C55DBD443E9D624D08EFCC14D6ED292BEF95CE02E88A14C59DAD4595676CE38CEEBD1D8527DA5D00AF0198AAE9FC17A3B4E5D7901B4347948A8A6994A30FC7A68E3D4A59F18134490EF9  12F31B2C12F31B212F31B2C07301C9E9E
	 
2.5  DES ENCRYPTION

     DES encryption is done using encrypt command with DES mode
	 Below is details description of command
	 usage: 
     encrypt [[-m|--mode algorithm][-k|--key key][-p|--pad algorithm][-i|--iv vector][data [pattern..]* 

     -m|--mode algorithm   : The mode of operation.The size of the provided
                             key determines if single DES or triple DES with
                             2 or 3 keys has to be performed.
                             The following modes are supported:

                             'DES'     - Equivalent with 'DES/ECB'
                             'DES/ECB' - Perform an encryption/decryption task
							             in ECB mode (Electronic Code Book),
										 i.e. each input block is encrypted
                                         or decrypted on its own.
                             'DES/CBC' - Perform an encryption/decryption task
                                         in CBC mode(Cipher Block Chaining),
										 i.e. the input of any subsequent block
                                         depends on the cryptographic output of
                                         the previous block. 
            
     -k|--key key          : Key for encryption
	 
     -p|--pad padding
	      algorithm        : Defines the padding algorithm to be applied.
                             The following padding algorithms are available:
                             'NOPAD' - No padding will be performed
                             'M1'    - Right padded with as few as possible '0'
                                       bits necessary, even none, to obtain a
                                       data string whose length (in bits) is a
									   a positive integer multiple of the
                                       blocksize.
                             'M2'    - The data is right padded with a single 
                                       '1' bit plus as few as possible '0' bits
                                       necessary to obtain a data string whose
                                       length is a positive integer multiple of
									   the blocksize.
                             'M3'    - The data is right-padded with as few as
                                       possible '0' bits, even none, necessary
									   to obtain a data string whose length in
									   bits is a positive integer multiple of
                                       the blocksize. The resulting string is
									   than left-padded with a single block.
									   This block consists of the binary repres
									   entation of the length of the unpadded
                                       data string, left-padded with as few
									   bits, even none, as necessary to obtain
									   a block of blocksize length. The right
									   most bit of this block constitutes the
									   least significant bit of the binary repre
									   sentation of the encoded length.
                                       This parameter is optional,if it is not
									   present, it defaults to M2.            
     -i|--iv vector        : The initialization vector to seed the initial
	                         transformation function which is applied at the
                             beginning of the MAC algorithm.
     data                  : Data
                             pattern..  Description of expected result. Each
							 argument is pattern describing result as sequence
							 of hexdigits with embedded wild cards (*,?,[..]).
							 A leading ! char means NOT. Arguments '&' and '|'
							 are interpreted as the respective logical
							 operators. The result is ok, if it matches the
							 pattern and the logical expression.
							 
     Examples 1:
     crypto>encrypt –m DES/ECB 0011223344556677 NOPAD 1122334455667788