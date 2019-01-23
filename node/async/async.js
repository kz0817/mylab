"use string";

const sleep = (sec, name) => {
  console.log("[" + name + "] sleep: " + sec);
  return new Promise((resolve) => {
    setTimeout(() => resolve("[" + name + "] OK: " + sec), sec * 1000);
  });
}

const sleep1 = (name) => sleep(1, name);

const func1 = async (name) => {
  console.log(await sleep1(name));
  return await sleep(4, name);
}

console.log("start");
sleep(3, "A").then((res) => { console.log(res) });

console.log("func1");
func1("B").then((res) => { console.log(res) });

console.log("end");
