default: keygen

otp_enc: universal.h otp_enc.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o otp_enc universal.h otp_enc.c

otp_dec: universal.h otp_dec.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o otp_dec universal.h otp_dec.c

otp_enc_d: universal.h otp_enc_d.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o otp_enc_d universal.h otp_enc_d.c

otp_dec_d: universal.h otp_dec_d.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o otp_dec_d universal.h otp_dec_d.c

keygen: universal.h keygen.c
	gcc -Wall -std=c99 -D_POSIX_C_SOURCE -o keygen universal.h keygen.c

cleanall:
	rm otp_enc
	rm otp_dec
	rm otp_enc_d
	rm otp_dec_d
	rm keygen
