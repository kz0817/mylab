const sleep = (ms) => new Promise(resolve => setTimeout(resolve, ms));

const func1 = async () => {
    await sleep(1000);
    console.log('func1: 1');
    await sleep(1000);
    console.log('func1: 2');
    await sleep(1000);
    console.log('func1: 3');
};

const caller1 = () => {
  console.log("This method doesn't have async");
  console.log("A1");
  func1();
  console.log("A2");
};

const caller2 = async () => {
  console.log("This method has async");
  console.log("A1");
  await func1();
  console.log("A2");
};

const showHelp = () => {
  console.log('Usage:');
  console.log('');
  console.log('node async.js caller1|caller2');
  console.log('');
}

(() => {
  if (process.argv.length < 3) {
    showHelp();
    return;
  }
  const command = process.argv[2];
  const handlers = {
    'caller1': () => caller1(),
    'caller2': () => caller2(),
  };
  const handler = handlers[command];
  if (!handler) {
    console.log(`Unknown command: ${command}`);
    return;
  }
  handler();
})();
