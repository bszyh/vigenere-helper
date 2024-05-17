FROM yobasystems/alpine-nginx
ENV FLASK_APP=webapp.py

COPY app1_kasiski /usr/local/bin/app1_kasiski
COPY app2_chi /usr/local/bin/app2_chi
COPY webapp.py /webapp.py
COPY nginx.conf /etc/nginx/nginx.conf

RUN apk add py3-pip
RUN pip install Flask
CMD ["flask", "run", "--host=0.0.0.0"] & nginx -g "daemon off;"
