#!/bin/bash
# Update Doxygen documentation after push to 'master'.

set -e

: ${GITHUB_REPO:="alvisisme/tutorial-duilib"}
GITHUB_HOST="github.com"
GITHUB_CLONE="git://${GITHUB_HOST}/${GITHUB_REPO}"
GITHUB_URL="https://${GITHUB_HOST}/${GITHUB_PUSH-${GITHUB_REPO}}"

# if not set, ignore password
#GIT_ASKPASS="${TRAVIS_BUILD_DIR}/gh_ignore_askpass.sh"

skip() {
	echo "$@" 1>&2
	echo "Exiting..." 1>&2
	exit 0
}

abort() {
	echo "Error: $@" 1>&2
	echo "Exiting..." 1>&2
	exit 1
}

# TRAVIS_BUILD_DIR not set, exiting
[ -d "${TRAVIS_BUILD_DIR-/nonexistent}" ] || \
	abort '${TRAVIS_BUILD_DIR} not set or nonexistent.'

# check for pull-requests
[ "${TRAVIS_PULL_REQUEST}" = "false" ] || \
	skip "Not running Doxygen for pull-requests."

# check for branch name
[ "${TRAVIS_BRANCH}" = "master" ] || \
	skip "Running Doxygen only for updates on 'master' branch (current: ${TRAVIS_BRANCH})."

# check for job number
# [ "${TRAVIS_JOB_NUMBER}" = "${TRAVIS_BUILD_NUMBER}.1" ] || \
# 	skip "Running Doxygen only on first job of build ${TRAVIS_BUILD_NUMBER} (current: ${TRAVIS_JOB_NUMBER})."

# check doxygen version
doxygen_version()
{
	doxygen -v
}

doxygen_run()
{
	cd "${TRAVIS_BUILD_DIR}";
	doxygen ${TRAVIS_BUILD_DIR}/Doxyfile;
}

gh_pages_prepare()
{
	cd "${TRAVIS_BUILD_DIR}";
	[ ! -d "html" ] || \
		abort "Doxygen target directory already exists."
	git --version
	git clone --single-branch -b gh-pages "${GITHUB_CLONE}" html
	cd html
	# setup git config (with defaults)
	git config user.name "${GIT_NAME-travis}"
	git config user.email "${GIT_EMAIL-"travis@localhost"}"
	# clean working dir
	rm -f .git/index
	git clean -df
}

gh_pages_commit() {
	cd "${TRAVIS_BUILD_DIR}/html";
	git add --all;
	git diff-index --quiet HEAD || git commit -m "Automatic doxygen build";
}

gh_pages_push() {
	# check for secure variables
	[ "${TRAVIS_SECURE_ENV_VARS}" = "true" ] || \
		skip "Secure variables not available, not updating GitHub pages."
	# check for GitHub access token
	[ "${GH_TOKEN+set}" = set ] || \
		skip "GitHub access token not available, not updating GitHub pages."
	[ "${#GH_TOKEN}" -eq 40 ] || \
		abort "GitHub token invalid: found ${#GH_TOKEN} characters, expected 40."

	cd "${TRAVIS_BUILD_DIR}/html";
	# setup credentials (hide in "set -x" mode)
	git remote set-url --push origin "${GITHUB_URL}"
	git config credential.helper 'store'
	# ( set +x ; git config credential.username "${GH_TOKEN}" )
	( set +x ; [ -f ${HOME}/.git-credentials ] || \
			( echo "https://${GH_TOKEN}:@${GITHUB_HOST}" > ${HOME}/.git-credentials ; \
			 chmod go-rw ${HOME}/.git-credentials ) )
	# push to GitHub
	git push origin gh-pages
}

doxygen_version
gh_pages_prepare
doxygen_run
gh_pages_commit
gh_pages_push

