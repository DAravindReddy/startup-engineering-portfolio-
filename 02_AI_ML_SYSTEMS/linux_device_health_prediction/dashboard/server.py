"""
Linux Device Health AI Dashboard Server
Lightweight Flask REST server providing real-time telemetry streaming,
anomaly injection endpoints, and web visualization.
"""

import os
import sys
from flask import Flask, jsonify, render_template, request

# Ensure project root is in sys.path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from linux_agent.daemon import LinuxHealthAgent

app = Flask(__name__, template_folder="templates", static_folder="static")

# Shared agent instance
agent = LinuxHealthAgent(synthetic=True)


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/telemetry")
def get_telemetry():
    """Fetches the latest cycle record."""
    record = agent.run_cycle()
    return jsonify(record)


@app.route("/api/inject_anomaly", methods=["POST"])
def inject_anomaly():
    """Allows web UI to trigger synthetic faults."""
    data = request.get_json(silent=True) or {}
    anomaly_type = data.get("anomaly_type", "")

    if anomaly_type == "clear":
        agent.collector.clear_anomaly()
        return jsonify({"status": "cleared", "message": "Nominal streaming restored"})

    try:
        agent.collector.inject_anomaly(anomaly_type, duration_ticks=15)
        return jsonify({"status": "injected", "anomaly_type": anomaly_type})
    except ValueError as e:
        return jsonify({"status": "error", "message": str(e)}), 400


@app.route("/api/history")
def get_history():
    """Returns recent telemetry history."""
    return jsonify(agent.history)


def main():
    port = int(os.environ.get("PORT", 8082))
    print(f"[*] Starting Linux Device Health AI Dashboard on http://127.0.0.1:{port}")
    app.run(host="0.0.0.0", port=port, debug=False)


if __name__ == "__main__":
    main()
