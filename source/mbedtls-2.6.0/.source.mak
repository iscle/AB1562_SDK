# ==============================================================================
# add all .S and .c files in sub-folder of src/bt/CONTROLLER/ into target source
# ==============================================================================

C_SRC		+=	\
$(addprefix source/mbedtls-2.6.0/library/, \
platform.c \
rsa.c \
bignum.c \
oid.c \
md.c \
asn1parse.c \
sha256.c \
md_wrap.c \
aes.c \
chacha20.c \
platform_util.c \
)
