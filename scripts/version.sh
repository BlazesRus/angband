#!/bin/sh
#
# This scripts adds local version information from git.
#
# Because of the assumed path to the version stamp file in the top-level
# directory of the source tree and because it needs access to the git
# repository, the argument to this script should be the src directory for
# Angband or, if no argument is used, the working directory when this script
# is invoked should be an immediate subdirectory of the top-level directory of
# the source tree.
#
# Copied from Linux 2.6.32 scripts/setlocalversion and modified
# slightly to work better for OpenOCD, then taken and ripped to
# pieces for angband
#

usage() {
	echo "Usage: $0 [srctree]" >&2
	exit 1
}

cd "${1:-.}" || usage

# Check for git and a git repo.
if head=$(git rev-parse --verify --short HEAD 2>/dev/null); then
	# If we are past a tagged commit (like "v3.2.0-64-g72357d5"),
	# we pretty print it.
	if atag="$(git describe 2>/dev/null)"; then
		printf "$atag"

	# If we don't have a tag at all we print -g{commitish}.
	else
		printf '%s%s' -g $head
	fi

	# Check for uncommitted changes
	if git diff-index --name-only HEAD \
	    | read dummy; then
		printf '%s' -dirty
	fi

	# All done with git
	exit
fi

# There's no recognized repository; we must be a snapshot, and should have a
# version file generated by the snapshot process
cat ../version
