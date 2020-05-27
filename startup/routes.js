const helmet = require('helmet');
const express = require('express');
const headers = require('../middleware/headers');
const users = require('../routes/users');
const auth = require('../routes/authorization');
// const confirmation = require('../routes/email-confirmation');
// const reset = require('../routes/reset-password');

module.exports = (app) => {
  app.use(helmet());
  app.use(express.json());
  app.use(headers);
  app.use('/api/users', users);
  app.use('/api/auth', auth);
  //   app.use('/api/confirmation', confirmation);
  //   app.use('/api/reset-password', reset);
};
