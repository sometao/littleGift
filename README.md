### Little Gift v0.1.1
---

#### Goals
- A easy way to presentation.
- Try using http in C++.
- Learning how to use database.

#### instructions
- Markdown text to slides.
- Three empty lines for a new horizontal slide.
- Two empty lines for a new vertical slide.
- Input access code to make your slides secret.
- Web Slide powered by [reveal.js](https://github.com/hakimel/reveal.js)


#### Features
- [X] Post markdown text and get slides page.
- [X] Keep markdown in DB
- [X] Preview slides
- [X] Code to access slides page
- [ ] Delete data after expired
- [ ] User system
- [ ] Edit exist slides
- [ ] Local static server for html develop


#### export to pdf
1. add `print-pdf` parameter to URL.
e.g:
```
http://HOST:PORT/littleGift/gift?token=fc0cb9aced3bd3c08175c4922292e3b1b0a21e9f
```
to
```
http://HOST:PORT/littleGift/gift?token=fc0cb9aced3bd3c08175c4922292e3b1b0a21e9f&print-pdf
```
2. Open the in-browser print dialog (CTRL/CMD+P).
3. Change the Destination setting to Save as PDF.
4. Change the Layout to Landscape.
5. Change the Margins to None.
6. Enable the Background graphics option.
7. Click Save

#### Pages
- [X] Editor
- [X] Preview
- [ ] Slides information 
- [X] Presentation
- [ ] Home


#### Try and learn.
- [X] Get Html from server
- [X] Post form data
- [X] Post Json data
- [ ] Try Cookie
- [ ] Login / Logout
- [ ] upload file and save it in server.
- [X] save data in DB.


#### TODO 
- [X] Write instructions.
- [ ] Using configure file.
- [ ] Preview without http request.
- [ ] Optimize the editor experience.