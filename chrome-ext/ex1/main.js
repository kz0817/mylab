(() => {
  console.log('Start');

  const getElement = (id) => {
    const elem = document.getElementById(id);
    if (elem == null) {
      console.log('Failed to find: ' + id);
    }
    return elem;
  }

  const getTimeStamp = () => {
    const now = new Date();
    const ret =
      ('0' + now.getHours()).slice(-2) + ':' +
      ('0' + now.getMinutes()).slice(-2) + ':' +
      ('0' + now.getSeconds()).slice(-2) + '.' +
      ('0' + now.getMilliseconds()).slice(-3);
    return ret;
  }

  const log = (msg) => {
    const area = getElement('info-area');
    const msgElem = document.createElement('div');
    msgElem.innerHTML = '<tt>' + getTimeStamp() + '</tt> ' + msg;
    area.insertBefore(msgElem, area.firstChild);
  }

  const logError = (msg) => {
    log('[ERROR] ' + msg);
  }

  const readURL = (url, successCb) => {
    log('Trying to connect: ' + url);
    const xhr = new XMLHttpRequest();
    xhr.onload = () => {
      successCb({
        res: JSON.parse(xhr.responseText),
      });
    }
    xhr.onerror = () => {
      logError('status: ' + xhr.status + ' (' + xhr.statusText + ')');
    }
    xhr.open('GET', url);
    xhr.send(null);
  }

  document.addEventListener('DOMContentLoaded', () => {
    const runButton = getElement('run-button');
    runButton.onclick = () => {
      readURL(getElement('url-box').value, (param) => {
        log(param.res);
      });
      return false;
    };
  });
})();
