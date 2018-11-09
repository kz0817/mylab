sample.txt.enc: sample.txt passwd.txt
	openssl enc -e -aes-256-cbc -in $< -out $@ -pass file:passwd.txt

.PHONY: show

show: sample.txt.enc
	openssl enc -d -aes-256-cbc -in $< -out /dev/null -pass file:passwd.txt -p
