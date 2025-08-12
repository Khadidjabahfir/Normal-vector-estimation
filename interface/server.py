from flask import Flask, request, jsonify
import subprocess
# fix the cors problem 
from flask_cors import CORS
import os 


app = Flask(__name__)
CORS(app)
@app.route('/')
def home():
    return "Hello from Flask!"

@app.route("/run_cpp", methods=["POST"])
def run_cpp():
    
    points = request.json.get("points")

    if not points:
        return jsonify({"error": "No points provided"}), 400

    
    input_data = str(points) 


    result = subprocess.run(
        ["../main"], 
        input=input_data.encode(), 
        stdout=subprocess.PIPE, 
        stderr=subprocess.PIPE
    )

    return jsonify({
        "stdout": result.stdout.decode(),
        "stderr": result.stderr.decode()
    })

if __name__ == "__main__":
    port = int(os.environ.get('PORT', 10000))
    
    # CRITICAL: Bind to 0.0.0.0, not 127.0.0.1
    app.run(host='0.0.0.0', port=port, debug=False)

