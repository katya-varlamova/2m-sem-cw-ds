from jwcrypto import jwk
import json
for i in range(5):
    key = jwk.JWK.generate(kty='RSA', size=2048, alg='RS256', use='sig', kid=f'{i}')
    public_key = key.export_public()
    private_key = key.export_to_pem(private_key=True, password = None)
    print(public_key)
    print(private_key)
    print()
##from cryptography.hazmat.primitives import serialization
##from cryptography.hazmat.primitives.asymmetric import rsa
##from cryptography.hazmat.backends import default_backend
##import json
##
### Generate 5 RSA key pairs
##key_pairs = []
##for _ in range(1):
##    private_key = rsa.generate_private_key(
##        public_exponent=65537,
##        key_size=2048,
##        backend=default_backend()
##    )
##    public_key = private_key.public_key()
##    
##    # Serialize the private key to PEM format
##    private_key_pem = private_key.private_bytes(
##        encoding=serialization.Encoding.PEM,
##        format=serialization.PrivateFormat.TraditionalOpenSSL,
##        encryption_algorithm=serialization.NoEncryption()
##    )
##    
##    # Serialize the public key to PEM format
##    public_key_pem = public_key.public_bytes(
##        encoding=serialization.Encoding.PEM,
##        format=serialization.PublicFormat.SubjectPublicKeyInfo
##    )
##    #print(public_key.public_bytes(encoding = ,))
##    
##    key_pairs.append((public_key_pem.decode(), private_key_pem.decode()))
##
### Create JWKS
##jwks = {
##    "keys": [
##        {
##            "kty": "RSA",
##            "kid": str(i),
##            "use": "sig",
##            "alg": "RS256",
##            "e": "AQAB",
##            "n": public_key_pem
##        } for i, (public_key_pem, _) in enumerate(key_pairs)
##    ]
##}
##
### Convert JWKS to JSON string
##jwks_json = json.dumps(jwks)
##
### Print JWKS and private keys
##print("JWKS:")
##print(jwks_json)
##
##for i, (_, private_key_pem) in enumerate(key_pairs):
##    print(f"Private Key {i+1}:")
##    print(private_key_pem)
