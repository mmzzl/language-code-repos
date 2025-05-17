module.exports = {
  apps: [
    {
      name: 'celery-worker',
      script: 'python',
      args: '-m celery -A videoproject worker --loglevel=info --pool=solo',
      cwd: '/home/fantom/videoproject',
      env: {
        PYTHONPATH: '/home/fantom/videoproject'
      },
      interpreter: 'none', // 不要用 node.js 来执行
      autorestart: true,
    },
    {
      name: 'celery-flower',
      script: 'python',
      args: '-m celery -A videoproject flower --port=5555',
      cwd: '/home/fantom/videoproject',
      env: {
        PYTHONPATH: '/home/fantom/videoproject'
      },
      interpreter: 'none',
    }
  ]
};