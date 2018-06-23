const crypto = require('crypto');
const fs = require('fs');

const SALT_HEADER_SIZE = 8;
const SALT_BODY_SIZE = 8;
const SALT_SIZE = SALT_HEADER_SIZE + SALT_BODY_SIZE;

const AES_BLOCK_SIZE = 16;

const showHex = (title, buf) => console.log(title + ': ' + buf.toString('hex'));

const decrypt = (encdata, password) => {
  showHex('data: ', encdata);
  showHex('passwd: ', password);

  const salt = encdata.slice(SALT_HEADER_SIZE, SALT_SIZE);
  showHex('salt: ', salt);

  const saltpasswd = Buffer.concat([password, salt]);

  const key = crypto.createHash('sha256').update(saltpasswd).digest();
  showHex('key: ', key);

  const iv = crypto.createHash('sha256')
              .update(Buffer.concat([key, saltpasswd])).digest()
              .slice(0, AES_BLOCK_SIZE);
  showHex('IV: ', iv);

  const algorithm = 'aes-256-cbc';
  const decipher = crypto.createDecipheriv(algorithm, key, iv)
  const encBody = encdata.slice(SALT_SIZE);
  const dec = Buffer.concat([decipher.update(encBody) , decipher.final()]);
  console.log('decrypted: ' + dec.toString('utf-8'));
};

const main = () => {
  const encdata = fs.readFileSync('data/sample.txt.enc');
  const passwd = fs.readFileSync('data/passwd.txt');
  decrypt(encdata, passwd);
};

main()
