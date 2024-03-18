module.exports = {
	extends: ['@commitlint/config-conventional'],
	rules: {
		'body-max-line-length': [2, 'always', Infinity],
		'header-max-length': [2, 'always', 200],
	},
};
