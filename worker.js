let throng = require('throng');
let Queue = require('bull');

const REDIS_URL = process.env.REDIS_URL || 'redis://127.0.0.1:6379';

const workers = process.env.WEB_CONCURRENCY || 2;

const maxJobsPerWorker = 1;

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function start() {
  let workQueue = new Queue('work', REDIS_URL);

  workQueue.process(maxJobsPerWorker, async (job) => {
    // This is an example job that just slowly reports on progress
    // while doing no work. Replace this with your own job logic.
    let progress = 0;

    while (progress < 100) {
      await sleep(50);
      progress += 1;
      job.progress(progress);
    }

    // A job can return values that will be stored in Redis as JSON
    // This return value is unused in this demo application.
    return { value: 'This will be stored' };
  });
}

// Initialize the clustered worker process
// See: https://devcenter.heroku.com/articles/node-concurrency for more info
throng({ workers, start });
