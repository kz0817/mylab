'use strict'
const express = require('express')
const cookieParser = require('cookie-parser')
const page1 = require('./routes/page1')
const app = express()

app.use(cookieParser())

app.set('views', './views')
app.set('view engine', 'pug')

app.use((req, res, next) => {
  next()
});

app.get('/', (req, res, next) => {
  const sessionId = req.cookies.sessionId
  if (!sessionId)
    res.redirect('login');
  else
    res.render('index')
})

app.get('/login', (req, res, next) => {
    res.render('login')
})

app.post('/login', (req, res, next) => {
  res.cookie('sessionId', 'test-id')
  res.redirect('/')
})

app.get('/logout', (req, res, next) => {
  res.clearCookie('sessionId');
  res.redirect('/')
})

app.use('/page1', page1)

const server = app.listen(3000, () => {
    console.log('Node.js is listening to PORT:' + server.address().port)
})

