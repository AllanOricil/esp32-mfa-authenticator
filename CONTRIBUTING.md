# Contribution Guidelines

Thank you for considering contributing to this project! We welcome all types of contributions, from reporting bugs to suggesting enhancements and writing code. To ensure a smooth and efficient collaboration, please follow these guidelines.

## Table of Contents

- [Contribution Guidelines](#contribution-guidelines)
  - [Table of Contents](#table-of-contents)
  - [Code of Conduct](#code-of-conduct)
  - [How to Contribute](#how-to-contribute)
    - [Reporting Bugs](#reporting-bugs)
    - [Suggesting Enhancements](#suggesting-enhancements)
  - [Contributing Code](#contributing-code)
    - [Development Workflow](#development-workflow)
    - [Coding Standards](#coding-standards)
    - [Commit Messages](#commit-messages)
      - [Example](#example)
    - [Pull Request Process](#pull-request-process)
    - [License](#license)

## Code of Conduct

By participating in this project, you agree to abide by our [Code of Conduct](CODE_OF_CONDUCT.md). Please take a moment to review it.

## How to Contribute

### Reporting Bugs

If you find a bug, please create an issue using the [bug report template](.github/ISSUE_TEMPLATE/bug_report.md). Provide as much information as possible to help us reproduce and fix the issue quickly.

### Suggesting Enhancements

If you have an idea for a new feature or an improvement, please create an issue using the [feature request template](.github/ISSUE_TEMPLATE/feature_request.md). Describe your idea in detail so we can discuss it.

## Contributing Code

### Development Workflow

1. Fork the repository.
2. Clone your fork.
   ```bash
   git clone https://github.com/allanoricil/esp32-mfa-authenticator.git
   ```
3. Create a new branch for your feature or bugfix.
   ```bash
   git checkout -b feature-or-bugfix-name
   ```
4. Make your changes.
5. Commit your changes following our [Commit Messages](#commit-messages) guidelines.
6. Push your branch to your fork.
   ```bash
   git push origin feature-or-bugfix-name
   ```
7. Open a pull request against the `main` branch of the original repository.

### Coding Standards

- Follow the existing code style.
- Write clear, concise, and well-documented code.
- Include unit tests for any new functionality or changes.
- Ensure all tests pass before submitting your pull request.

### Commit Messages

This project uses [Conventional Commits](https://www.conventionalcommits.org/v1.0.0/). Please, format your commit message as follows:

- <b>feat</b>: A new feature
- <b>fix</b>: A bug fix
- <b>docs</b>: Documentation only changes
- <b>style</b>: Changes that do not affect the meaning of the code (white-space, formatting, missing semi-colons, etc)
- <b>refactor</b>: A code change that neither fixes a bug nor adds a feature
- <b>perf</b>: A code change that improves performance
- <b>test</b>: Adding missing or correcting existing tests
- <b>build</b>: Changes that affect the build system or external dependencies (example scopes: gulp, broccoli, npm)
- <b>ci</b>: Changes to our CI configuration files and scripts (example scopes: Travis, Circle, BrowserStack, SauceLabs)
- <b>chore</b>: Other changes that don't modify src or test files
- <b>revert</b>: Reverts a previous commit

#### Example

```bash
feat: add user authentication

- Added JWT-based authentication
- Updated user model with authentication methods
- Created login and registration endpoints
```

### Pull Request Process

1. Ensure your code follows the [Coding Standards](#coding-standards).
2. Verify all tests pass and add new tests if applicable.
3. Describe the purpose of the pull request in the description.
4. Reference any related issues.
5. One of the maintainers will review your pull request. Please be responsive to any feedback.

### License

By contributing, you agree that your contributions will be licensed under the project's [MIT License](https://github.com/AllanOricil/esp32-mfa-authenticator/blob/main/LICENSE).

Thank you for your contributions! We look forward to collaborating with you.
