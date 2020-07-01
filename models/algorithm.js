const Joi = require('joi');

const kmeansSchema = {
  algorithm: Joi.number().required(),
  data: {
    dimensions: Joi.number().min(1),
    values: Joi.array()
      .items(
        Joi.array()
          .length(Joi.ref('dimensions'))
          .items(Joi.number().required())
          .required()
      )
      .required(),
  },
};

const validateAlgorithm = (data, schema) => {
  return Joi.validate(data, schema);
};

module.exports = {
  kmeansSchema,
  validateAlgorithm,
};
