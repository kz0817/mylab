"use string";

const sleep = (sec) => {
  console.log("sleep: " + sec);
  return new Promise((resolve) => {
    setTimeout(() => resolve('OK: ' + sec), sec * 1000);
  });
}

const func1 = async () => {
  return await sleep(2);
}

console.log("start");
sleep(3).then((res) => { console.log(res) });

console.log("func1");
func1().then((res) => { console.log(res) });

console.log("end");
