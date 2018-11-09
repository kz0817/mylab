const crypto = require('crypto')
const fs = require('fs')

const SALT_HEADER_SIZE = 8
const SALT_BODY_SIZE = 8
const SALT_SIZE = SALT_HEADER_SIZE + SALT_BODY_SIZE

const AES256_KEY_SIZE = 32
const AES256_BLOCK_SIZE = 16
const HASH_ALGO = 'sha256' // Old openssl uses md5
const HASH_SIZE = 32

const showHex = (title, buf) => console.log(title + ': ' + buf.toString('hex'))

const divCeil = (a, b) => (a + b - 1) / b

const decrypt = (encdata, password) => {
  const algorithm = 'aes-256-cbc';
  console.log('data: ' + encdata.toString('hex'))
  console.log('passwd: ' + password)

  const salt = Buffer.allocUnsafe(SALT_BODY_SIZE)
  encdata.copy(salt, 0, SALT_HEADER_SIZE, SALT_SIZE)
  console.log('salt: ' + salt.toString('hex'))

  const saltpasswd = Buffer.concat([password, salt])
  const hash = new Array()
  hash[0] = Buffer.allocUnsafe(0)
  const num_loop = divCeil(AES256_KEY_SIZE + AES256_BLOCK_SIZE, HASH_SIZE)
  for (let i = 1; i <= num_loop; i++) {
    data = Buffer.concat([hash[i-1], saltpasswd])
    hash[i] = crypto.createHash(HASH_ALGO).update(data).digest()
    showHex('hash[' + i + '] ', hash[i])
  }

  const actualPasswd = hash[1]
  console.log('actualPasswd: ' + actualPasswd.toString('hex'))

  const iv = Buffer.allocUnsafe(AES256_BLOCK_SIZE)
  hash[2].copy(iv, 0, 0, AES256_BLOCK_SIZE)
  console.log('iv: ' + iv.toString('hex'))

  const decipher = crypto.createDecipheriv(algorithm, actualPasswd, iv)

  const encBodySize = encdata.length - SALT_SIZE;
  const encBody = Buffer.allocUnsafe(encBodySize)
  encdata.copy(encBody, 0, SALT_SIZE)
  const dec = Buffer.concat([decipher.update(encBody) , decipher.final()]);
  console.log('\ndecrypted: ' + dec.toString('utf-8'))
}

const main = () => {
  const encdata = fs.readFileSync('data/sample.txt.enc')
  const passwd = fs.readFileSync('data/passwd.txt')
  decrypt(encdata, passwd)
};

main()
