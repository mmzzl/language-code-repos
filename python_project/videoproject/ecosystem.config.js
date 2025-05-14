module.exports = {
  apps: [
    {
      name: 'celery-worker',
      script: 'python',
      args: '-m celery -A videoproject worker --loglevel=info --pool=solo',
      cwd: 'D:\\new_apps\\language-code-repos\\python_project\\videoproject',
      env: {
        PYTHONPATH: 'D:\\new_apps\\language-code-repos\\python_project\\videoproject'
      },
      interpreter: 'none', // 不要用 node.js 来执行
      autorestart: true,
    },
    {
      name: 'celery-flower',
      script: 'python',
      args: '-m celery -A videoproject flower --port=5555',
      cwd: 'D:\\new_apps\\language-code-repos\\python_project\\videoproject',
      env: {
        PYTHONPATH: 'D:\\new_apps\\language-code-repos\\python_project\\videoproject'
      },
      interpreter: 'none',
    }
  ]
};