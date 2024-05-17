FROM yobasystems/alpine-nginx

# Installing dependencies
RUN apk add --no-cache py3-pip
RUN pip install Flask

# Setting env variable
ENV FLASK_APP=webapp.py

# Copy app files
COPY app1_kasiski /usr/local/bin/app1_kasiski
COPY app2_chi /usr/local/bin/app2_chi
COPY webapp.py /webapp.py
COPY nginx.conf /etc/nginx/nginx.conf

# Setting the entry point
CMD ["sh", "-c", "flask run --host=0.0.0.0 & nginx -g 'daemon off;'"]
