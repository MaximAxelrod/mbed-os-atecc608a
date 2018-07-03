/* ATECC608A driver
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ATCAKEY_H
#define ATCAKEY_H

#include "CryptoEngineInterface.h"
#include "ATCAConstants.h"
#include "ATCAError.h"

/** Key class representing an ECC Key pair stored in an ATCA device.
 */
class ATCAKey {
private:
    CryptoEngineInterface & device; /** Device driver reference */
    ATCAKeyID               keyId;  /** Key ID vis-a-vis slot number */
    uint8_t                 pk[ATCA_ECC_ECC_PK_LEN];/** Public key X & Y buffer */
public:
    /** Instantiate Key object with device reference, Key Id and public key.
     *
     *  Key object uses device reference for sign and verify functions using
     *  private key stored in the device slot referred by the Key Id and the
     *  input pubic key respectively.
     *  Public key can be re-calculated from the device. It is retrieved
     *  outside this class to avoid failure in this constructor or having
     *  an incomplete state object.
     *
     *  @param dev      Device driver reference.
     *  @param keyId_in Key Id/Slot number in device data zone.
     *  @param pk       Public key input buffer with X & Y concatenated.
     */
    ATCAKey(CryptoEngineInterface & dev, ATCAKeyID keyId_in, uint8_t * pk);

    /** Sign input hash with ECC private key stored at Key Id and output the
     *  signature.
     *
     *  @param hash     Input hash buffer.
     *  @param hash_len hash buffer length.
     *  @param sig      Out signature buffer.
     *  @param sig_buf_len  Signature buffer length.
     *  @param sig_len  Out signature length.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError Sign( const uint8_t * hash, size_t hash_len, uint8_t * sig,
                    size_t sig_buf_len, size_t * sig_len);

    /** Verify input signature with input hash. Signature is supposedly
     *  generated by an SSL peer using the same public key. Device's verify
     *  function re-generates signature using input hash and public key and
     *  matches with the input signature to verify if it is correct.
     *   Public key is provided to the device as a command input. This
     *  function does not provide any security rather it works as an off chip
     *  crypto accelerator.
     *
     *  @param hash     Input hash buffer.
     *  @param hash_len hash buffer length.
     *  @param sig      Out signature buffer.
     *  @param sig_len  Out signature length.
     *  @return         Error code from enum ATCAError.
     */
    ATCAError Verify( const uint8_t *hash, size_t hash_len,
                      const uint8_t *sig,
                      size_t sig_len );

    /** Get Public key that can be required by a certificate creation app.
     *
     *  @return Pointer to Public key buffer.
     */
     uint8_t * GetPubKey();
};

#endif /* ATCAKEY_H */

