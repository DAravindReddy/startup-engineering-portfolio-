#!/usr/bin/env python3
"""
Android Device Health Web Dashboard Server
Provides REST telemetry endpoints and an interactive real-time dashboard UI.
"""

import json
import os
import sys
from flask import Flask, jsonify, render_template, request

# Ensure local imports work
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "..")))

from application.health_agent import AndroidHealthAgent

app = Flask(__name__, template_folder="templates", static_folder="static")

# Initialize global health agent
agent = None
current_anomaly_mode = None


def get_agent():
    global agent
    if agent is None:
        model_path = os.path.join(os.path.dirname(__file__), "../inference_engine/model.joblib")
        if not os.path.exists(model_path):
            from model_training.isolation_forest.train import main as train_main
            train_main()
        agent = AndroidHealthAgent(force_mock=True, model_path=model_path)
    return agent


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/api/telemetry")
def get_telemetry():
    global current_anomaly_mode
    health_agent = get_agent()
    result = health_agent.step(inject_anomaly=current_anomaly_mode)
    return jsonify(result)


@app.route("/api/set_anomaly", methods=["POST"])
def set_anomaly():
    global current_anomaly_mode
    data = request.get_json(silent=True) or {}
    mode = data.get("anomaly", None)
    current_anomaly_mode = mode if mode in ["thermal", "memory_leak", "cpu_hog", "subsystem_stall"] else None
    return jsonify({"status": "success", "current_anomaly": current_anomaly_mode})


if __name__ == "__main__":
    port = 5050
    print(f"[*] Starting Android Health Prediction Dashboard on http://localhost:{port}/")
    app.run(host="0.0.0.0", port=port, debug=False)
