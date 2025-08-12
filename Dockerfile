FROM python:3.10-slim

ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Copy files FIRST
COPY . /app/

# Debug: Show what files we copied
RUN echo "=== Files after copy ==="
RUN ls -la

# Debug: Show Makefile contents
RUN echo "=== Makefile contents ==="
RUN cat Makefile || echo "No Makefile found"

# Run make and show output
RUN echo "=== Running make ==="
RUN make || echo "Make failed"

# Debug: Show files after make
RUN echo "=== Files after make ==="
RUN ls -la

# Install Python dependencies
RUN pip install flask flask-cors

# Try to make executable (this might fail if main doesn't exist)
RUN chmod +x ./interface/server.py
RUN chmod +x ./entrypoint.sh
RUN chmod +x main || echo "Could not chmod main - file doesn't exist"

EXPOSE $PORT

ENTRYPOINT ["./entrypoint.sh"]