FROM python:3.10-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Copy files first
COPY . /app/

# Install Python dependencies
RUN pip install flask flask-cors

# Make scripts executable
RUN chmod +x ./interface/server.py
RUN chmod +x ./entrypoint.sh

# Use the PORT environment variable that Render provides
EXPOSE $PORT

ENTRYPOINT ["./entrypoint.sh"]