#!/usr/bin/env bash
set -euo pipefail

# Always run from the script location
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "Pre-step: stopping any existing python3 -m http.server 9000"
if command -v pkill >/dev/null 2>&1; then
	pkill -f "python3 -m http.server 9000" 2>/dev/null || true
else
	echo "pkill not available; skipping pre-stop check."
fi

echo "Step 1: cd _build/html"
cd "_build/html"

echo "Step 2: rm -rf _build/"
# Return to project root so _build/ path is correct
cd "$SCRIPT_DIR"
rm -rf "_build/"

echo "Step 3: ./build.sh"
./build.sh

echo "Step 4: cd _build/html/"
cd "_build/html/"

echo "Step 5: python3 -m http.server 9000"
echo "Press 'q' to stop the server and quit this script."

python3 -m http.server 9000 &
SERVER_PID=$!

cleanup() {
	if kill -0 "$SERVER_PID" 2>/dev/null; then
		kill "$SERVER_PID" 2>/dev/null || true
		wait "$SERVER_PID" 2>/dev/null || true
	fi
}

trap cleanup EXIT INT TERM

while kill -0 "$SERVER_PID" 2>/dev/null; do
	if read -r -s -n 1 key; then
		if [[ "$key" == "q" || "$key" == "Q" ]]; then
			echo
			echo "'q' pressed. Stopping server..."
			cleanup
			break
		fi
	fi
done
