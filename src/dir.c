#include "dir.h"

static const char *dir_page =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
"<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">\n"
"<head>\n"
/* css */
"  <style type='text/css'>\n"
"/*<![CDATA[*/\n"
"  body { background-color: white; }\n"
"  #content { border:2px solid #000000; margin:5em auto 10px auto; font-family:Georgia, Times, serifs; width:600px; padding: 30px 50px; }\n"
"  #top { font-weight:bold; padding: 7px 0; }\n"
"  div.ico { width:16px; height:16px; float:left; }\n"
"  div.filename { width:300px; float:left; text-align:left; margin-left:10px; }\n"
"  div.modified { width:150px; float:left; text-align:left; }\n"
"  div.size { width:80px; float:right; text-align:right; }\n"
"  .clear { clear:both; }\n"
"  .bar { border-top:#000000 2px solid; margin-bottom:5px; clear:both; }\n"
/* arrow */
"  .arrow { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAAVElEQVR42qWT0QoAIAgD9/8fbYEQSGLONrC3O1IRZraUgj/AV0kC6QdSC9IMInhCC6pQgipUC12eAialgImzbhn03K5xAidB3G8HJsENj64xwtNz3iu3wU8Fir11AAAAAElFTkSuQmCC); }\n"
/* texts */
"  .file { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABYlBMVEUgIGVZgcZYgsVdhMRehMRhiMNiicNlisJni8FpjMFsjsBujr9ykL9ykb92k716lbt7lbx+l7qCmryFnb6IoMCLosKMosKPpcSRp8aSqMaVq8iXrMmarsudsM2dsc2htM6jttGkttGnudOpu9SqvNWuvtevwNizw9rU5PrW5PrW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/vc6Pvc6Pzc6fvc6fzd6fvd6fze6fve6vve6vzf6vvf6vzf6/zg6/vg6/zh6/zg7Pzh7Pvh7Pzi7Pvi7Pzi7fzj7fvj7fzk7fzj7vzj7v3k7vzl7vzl7v3l7/zl7/3m7/zm7/3n7/zn8Pzn8P3o8Pzo8P3p8P3o8f3p8f3q8f3r8f3q8vzr8v3r8/3s8/3t8/3t9P3t9P7u9P3u9P7v9P3u9f7v9f3v9f7w9f7w9v3x9v3x9/7y9/3y9/7z9/70+P7///9VgMh2qvBUAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAGCK9tcJOAAAA50lEQVQY02NgYGBQV1aQk5YQFRYUZIAAteLiooLc7DTBEqiIanFhXk5GarJgCVREpTA/Nz0lKQEoABFRysnJTE6KjxEEAZCAYlZaUmJsTGRwgJcHWEA+LSkhNjo8xN/XwwUsIJuUEBMRGuzn4+rqyA8SkImLigz39/Nydba35gMJSEWGB/t7e7g6WluZ84AEJMMCA7zdXR2sLUyMuEAC4kH+Hu5O9jZmJkYGHCABMW9PN0d7G1MTQz0dNpCAiIerva2libGhvq4WC0hA2NHOCqhdX09bU4MRJCAkIMDHy83JzsrMVFoKAB5KMY+XnxuDAAAAAElFTkSuQmCC); }\n"
"  .folder { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAYAAAAf8/9hAAAABmJLR0QA/wD/AP+gvaeTAAAABGdBTUEAALGOfPtRkwAAACBjSFJNAAB6JQAAgIMAAPn/AACA6AAAdTAAAOpgAAA6lwAAF2+XqZnUAAADiUlEQVR4nGLQMzJl0NIzYNDXVucw01ZWNzAwkNQxNAGKGQLFNDmsDTQNoxyNslpi7Nb0Rps+NzPSdzYyMWUwBuL///8zAAQQQ7S9VlFrrNOKte35Ny6t6PpbG2ZzLtxWL7cx2nHh2pakO1eXlvx+f7D5//9bvf/PT/L4r6upkairrclgbmwoAzIAIAAAQQC+/wClc0/i//bW///1wf//9sD/89av/+/HkP//25f//9iC//7Sc//1vVX/8rBF/9OGMP99UjnJrYBh1HJLLd3///8AAgBBAL7/AIJdRLj/56f/+ddv//jSb//813f/+Mlj//G2Tf/qqUn/455O/+Sqcf/nuZD/8NS2///50//87LX/Z0guw////wACiCHD06z3x9VZ/39dyPn/cV/c/yeLA/4/XVME1vD+0rb/P95e+P/31zmg2qP///9bC6Tn/X9xOPW/h7n2TJABAAHEEGJvnP56X+v/32fS/z+Z7/n/073z///+vg6UOw3Em/7//7Pg///vU/7//9j7//+bTiDd8f/8HK//prrauSADAAKIwdFU3+XuioL/v44k/n88P+z/r09X/v//Oxei6dPE///fdv///6Ll///Htf//3y////9B/v81Veb/9HX07UAGAAQQgxkwWE9PS/r+c1f4/8dLMv7/+XISaEvP//+vO/7/f9bw///Div//7xT+/3896///Kyn//58L+98dq/1RU89EEmQAQAAxGOpo8W1ujnjxdaXL/6drK////7z9//9HZf//3yv9//8m0JVX0/7/Px8H9FH4//8nA///2+X4P9tN7ZS2gRkLyACAAGLQ0dVnmV3gd/LTLLP/z7cBnft6NlBT8v//FxOAmiL//z8eCAw/LyB2+///kP3/Z1O1/ntbaM/XN4IkJIAAYtDSN2ZoirRc9WGWw//Xh4CabwCdeyLg//9jvkBN7v//H7D//3Od6f8bvZr/5yar/IuyU79gbGziYGJiAjYAIIAYtA1NGbI9tLueTvP6//n8/P//zwT9/3/Q/v+fTeb/707Q+r8oXeVfirPaZWtD3W5tPRNLXSMzZpBmmAEAAcQAygth9npZdztc/n873vv/Zp/G/2VZqv9z3NRu2xnr9unoGTvqGJqxGQLTPkwjsgEAAcRgZGzCYGNuZFnppfYy1UX7lI2hTouunpEzMJB4DI0xNaEbABBAYIISDBBAFBsAEGAAg6Rt6VBtWbkAAAAASUVORK5CYII=); }\n"
"  .html { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABfVBMVEVlIAkVLEIVLEMjLD4nMTweNlYiOFkmOV8ePWQcPmUfPG4eQHw3QjEhQn1ISDclUIImUptAUlYlUaYqWIwuXIpeWDEuXK1XWFRcWlAvYLBDZHE5YpszY7M3bqk6brk9dKQ5csxCdrxNeJxNeKlBes5Oe6aDdkBFfdBKhL5sgXlVgMhZgcZPhsdYgsVQhsdYhL5dhMRehMRJjtRhiMNiicNRjt9lisJYj9Rni8FpjMFvkKtsjsBujr9ykL9ykb9vl7h2k716lbt7lbx8m5quljd+l7pjoNqpmEqCmryFnb64mlOIoMCJorWLosKMosJ2qd2Aq9qRp8aerpWwsWiXrMmQrtadsc2jttHVt26pu9Taum+uvtfcvG+vwNjTwoazw9q4ybjk0IHy0nH30Yby15Xu3qvm3cvn4cbS4vrU4/rW5PrX5fvZ5vvw5sH+4L/b5/vc6Pve6fvg6vzh7Pzj7fzl7vz/8OXw9f7y9/7z9/3z9/7/9er0+P7///8XKUR12w5YAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMAgdyovnoAAAA6ElEQVQY02NgYGCIjwwPCwn09XR1ZYCA2BoIcK2FikRLc0pVVVZUuNZCRGTVkoJ0pDlDa4ECIBHZhLzcuGBNAcZyVxBgYOBPr05N9FIS4yoNDS0pdmXg10grS45RVpHhZSotKS5yZWAPsEpx9jFVl+RmLAotKnRkYNPzt4/SV5UTYmUsLioscGBgEDZ3M4pwF+dgLgoNLci3ZWCok1dUcFETZWEEyufnWAPdwcghIsjHAzQhNDQn2wLkMj9GRkYJsHx2lglIwLu0JBQCsjINQQKeJcVQ+cwMbZCAh5OTg52NpZmxga6WFgCqfj8jKC82iwAAAABJRU5ErkJggg==); }\n"
"  .htm { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABfVBMVEVlIAkVLEIVLEMjLD4nMTweNlYiOFkmOV8ePWQcPmUfPG4eQHw3QjEhQn1ISDclUIImUptAUlYlUaYqWIwuXIpeWDEuXK1XWFRcWlAvYLBDZHE5YpszY7M3bqk6brk9dKQ5csxCdrxNeJxNeKlBes5Oe6aDdkBFfdBKhL5sgXlVgMhZgcZPhsdYgsVQhsdYhL5dhMRehMRJjtRhiMNiicNRjt9lisJYj9Rni8FpjMFvkKtsjsBujr9ykL9ykb9vl7h2k716lbt7lbx8m5quljd+l7pjoNqpmEqCmryFnb64mlOIoMCJorWLosKMosJ2qd2Aq9qRp8aerpWwsWiXrMmQrtadsc2jttHVt26pu9Taum+uvtfcvG+vwNjTwoazw9q4ybjk0IHy0nH30Yby15Xu3qvm3cvn4cbS4vrU4/rW5PrX5fvZ5vvw5sH+4L/b5/vc6Pve6fvg6vzh7Pzj7fzl7vz/8OXw9f7y9/7z9/3z9/7/9er0+P7///8XKUR12w5YAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMAhtmo6WnAAAA6ElEQVQY02NgYGCIjwwPCwn09XR1ZYCA2BoIcK2FikRLc0pVVVZUuNZCRGTVkoJ0pDlDa4ECIBHZhLzcuGBNAcZyVxBgYOBPr05N9FIS4yoNDS0pdmXg10grS45RVpHhZSotKS5yZWAPsEpx9jFVl+RmLAotKnRkYNPzt4/SV5UTYmUsLioscGBgEDZ3M4pwF+dgLgoNLci3ZWCok1dUcFETZWEEyufnWAPdwcghIsjHAzQhNDQn2wLkMj9GRkYJsHx2lglIwLu0JBQCsjINQQKeJcVQ+cwMbZCAh5OTg52NpZmxga6WFgCqfj8jKC82iwAAAABJRU5ErkJggg==); }\n"
"  .css { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAB/lBMVEVbiQAxOkY0PEdCRk1ER01DSE5FSE5FSU5FSU9QVFtSVltTV1tUV1tYXWVYXmVZXmZZX2ZaX2ZcYGZVgMhZgcZYgsVdhMRehMRhiMNiicNlisJni8FpjMF3jKpsjsBujr9ykL9ykb92k716lbt7lbx+l7qEmLOCmryFnb6LosKRp8aUqseXrMmZrMarq6ygsciwsLCxsrKysrKxtLq3uLq3uby5urq6urq6u72+vr6/v7/AwMDBwcG4w9XCwsLDw8O7xdW8xtbGxse/x9bEx8rHx8fIyMjGyczJycnKysrKysvAzODLy8vJzdXNzc3Dz+HD0OPH0eHQ0NDH0uPJ0+PJ0+TK1OLP09vK1OTS09TT09PU1NTN1uPO1uTX19fS2OHN2e3L2u7O2uzO2u3P2+7P3O/S3e3S3e7c3NzS3u7S3u/d3d3U3+7V3+/e3t7X3+7W4O/Y4O/g4ODZ4e/Z4fDT4/nb4/DW5Prc4/Dg5Ork5OTY5vra5vrm5ubb6Pvh5/Pd6frh6fTe6vvf6vvf6/zg6/vg6/zh6/rq6urg7Pzh7Pvh7Pzi7fzj7fvj7fzk7fvs7Ozj7vzj7v3k7vzl7vvl7vzl7v3t7e3m7/zm7/3n7/zn8P3v7+/o8P3p8f3w8PDq8vzr8v3s8/3t8/3t9P7u9P3v9f77+/v///8sMz1Z+tfuAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAFDIMt50mAAAA+0lEQVQY02NgYGBw93LR19HS1FBVZQABTzvPOR5OkZXLlqquAIkYFU23t7afkG2xZLHqCpCIa15U3PI4T2uDRQuBAkARn/lV0UFWlnaGC+apggCDTUdtho+NjbXevLkzpvX1APWYRxf52ZmZxs+eOXVSTxfYImM3E9/GWTMmT+zublMGCeiG18+dPXVyX3dnS5MSSEC7omxKf2FBa3Nmep0CSECNh7sklosrJ4STM1UOJMAUU1wg5JyZJeCQXC0NEliZ39seIRhaE8wXUC4JEmAMy21Ic+RPSbLlTRQHCciycQSys/gnsDJ7l4qABNRVVJQU5WWkJMREhYUBJxZIKfpN8fsAAAAASUVORK5CYII=); }\n"
"  .js { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAB3VBMVEVYLbFER01DSE5FSE5FSU5GSk9GSlBHSlBQVFtTVltYXWVYXmVZX2ZVgMhZgcZYgsVdhMRehMRhiMNiicNlisJni8FpjMFsjsBujr9ykL9ykb92k716lbt7lbx+l7qCmryFnb6IoMCLosKMosKPpcSRp8aSqMaVq8iXrMmZrMarq6ygsciwsLCxsrKysrKxtLq3uLq3uby5urq6urq6u72+vr6/v7/AwMDBwcG4w9XCwsLDw8O7xdXGxsfEx8rHx8fIyMjGyczJycnKysrKysvAzODLy8vJzdXNzc3Dz+HD0OPG0eLQ0NDH0uLH0uPJ0uHJ0+PP09vS09TT09PU1NTX19fS2OHN2e3L2u7O2uzO2u3P2u3P3O/S3e7c3NzS3u7T3u3T3u7d3d3e3t7W3+/X3+7W4O/g4ODZ4e/T4/na4vHW5Prg5Ork5OTY5vrm5ubb6Pvh5/Ph6fTe6vvf6vvf6vzf6/zg6/vg6/zq6urg7Pzh7Pvh7Pzi7Pvi7fzj7fvj7fzk7fzs7Ozj7vzj7v3k7vzl7vvl7vzl7v3t7e3l7/zl7/3m7/zm7/3n7/zn8Pzn8P3v7+/o8P3p8f3w8PDq8vzr8v3s8/3t8/3t9P7u9P3v9f77+/v///9CRk39aEJDAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAGBB1YpPOAAAA+klEQVQY02NgYGBwdLHT1lBVUpCTYwABZyvniU62QTmzZ8nNBYnopXdam1s3JZnMnCE3FyRinxwcOifU2Vxn+jSgAFDEbUpuiI+ZqZXu1MlyIMBgUZkf52ZhYa41eVJPR0MdUI9xSLqHlZFh2ITe9pa6arBF+g4G7kV9Pa3NNTUVMiABzcDCSRPaWxtqqsqKpUEC6pMmtGWkppWXxMcWSIIE1Pq7sjhZ2RMCODhixEECKt3t1Sn+zL48NlF5oiAB5cb62op4tmg/bq9sYZCAYl1NcaI3R1SkJVeEIEhAoaI0loXRM5xpnmsmH0hAXlZWWkpCTERIgJ+XFwALWUR/+nMW1wAAAABJRU5ErkJggg==); }\n"
"  .xhtml { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAAulBMVEUAEK5ZgcZYgsVdhMRehMRhiMNiicNlisJni8FpjMH/ZgBsjsBuj9ykL9ykb92k716lbt7lbx+l7qCmyFnb6IoMCLosKMosKPpcSRp8aSqMaVq8iXMmQtaZcqdsc2jttGpu9SvwNizw9S4vU4/W5PX5fvZ5vvb5/vc6Pve6fvg6vzh7Pzj7fzl7vzm7/zo8P3q8f38v3t8/3v9P3w9v7y9/70+P71+f73+v75+/////9VgMjnfNz/AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhALOjjWFH6AAAAmklEQVQY01WNhxLCIBBEsSuWGIy9xJYOpNkl//9b5gLDmIWZY97uLQghdDkd7d16YRGCpM7fz/v1fNyJAIL/BQSL6mA5S6Ie1cWkAFLpQTmjNRWBKOkXkqjKXx7uOVZmsSc0Sg0AGzzbC8VBmMAG+0H/hDAKk1Uwvf6AJba99wugHnMVcJ12gAs7TvXBoCZaRqT0aDXaTWL4gcXdhAp9vAVQAAAABJRU5EkJggg==); }\n"
/* images */
"  .gif { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAACN1BMVEUJAQB5YFd1Y110Zmi5WESEbWSiZkeQb1SvaUtVgMiqbExXgcGFeXBZgcZYgsWSdXddhMRLpEpehMSCe5Z8gYi1b1Okd15hiMNiicNlisJni8GHhoWwe1ZpjMGLh45sjsC3eGtujr+4emRykL9ykb+5fmR2k72SjoyCnWq5gXB6lbuLj7B7lbyti2vOf1p+l7rEhGfMgly1jXSCmrytkIKtkX6/injifWChlZnTh1+Fnb7SiF3TiWDFkGnCkmmdnKOrlp2lmZ7PkGLDk3PYjGSTobC4loeLosKWn7vFlXHYkGWunZuRp8bKnHC7no+xopubq7vil2mXrMnMoJCbq97Mp4Kdsc3Pp4HkoG6yuIPlom/tnnPKqqGjttHNq57kqHbpqHOxw4LAtaqpu9Swu8WqvNXsrnnsr3qrwNSuvtfmsoKvwNjVt62vxNizw9rCyZPuuIS3yt3rv5Lvvo3twZDexqfpxZ7wxJX5w4jgyq290ufQ067iy6zyyJjzypzG1uLvzqDT09PX09H0z6LM2vHI3vL22rHU5Prt477R5vvW5Pr34LjW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/v25cHc6Pvc6Pzc6fvc6fzd6fve6fve6vvf6/zq6eng6/zg7Pzh7Pvh7Pz66sXi7fzi7vfj7fzj7v3l7v3w7ezy8fTt8/3s9Pzr9f7t9P7u9P3v9P3v9f7w9f7w9v3x9v39+Nfy9/3y9/7z9/70+P7///9mUkupYZeaAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMARTdMev1AAAAHXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBUaGUgR0lNUO9kJW4AAAECSURBVBjTY2BgYMhLjg0L8nG30tdngIDsXbt2bt+yaYP+bqhI5q4dWzdvXL9GfzdUJHV1TnBc09p1QAGIiIOBiW5FvN8yfRBgYFhlFu3MIxNobujfsnL5UqCApoewl1OCCwuHZ/2KpYv0GWIUuPmj0gqKS2qL7BcvXqDFoKRnHZGeVd6QGFkmt3DeXDUGUUsuMVtW03xBDaOAuXNmKTOo2uyxCE+q7KguZW6dPX2KPIMbm7R3XduSbTWMuTOnT5kkxTBfvdGxuXtaKFPGjOmTJ/RJgBymbefLLlI1ferkif09QiCBkBTXws7pUyZO6O1q5wUJGOvoqKkoykqKC/BxcgIAmKJUjBV48xsAAAAASUVORK5CYII=); }\n"
"  .jpg { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAACN1BMVEUJAQB5YFd1Y110Zmi5WESEbWSiZkeQb1SvaUtVgMiqbExXgcGFeXBZgcZYgsWSdXddhMRLpEpehMSCe5Z8gYi1b1Okd15hiMNiicNlisJni8GHhoWwe1ZpjMGLh45sjsC3eGtujr+4emRykL9ykb+5fmR2k72SjoyCnWq5gXB6lbuLj7B7lbyti2vOf1p+l7rEhGfMgly1jXSCmrytkIKtkX6/injifWChlZnTh1+Fnb7SiF3TiWDFkGnCkmmdnKOrlp2lmZ7PkGLDk3PYjGSTobC4loeLosKWn7vFlXHYkGWunZuRp8bKnHC7no+xopubq7vil2mXrMnMoJCbq97Mp4Kdsc3Pp4HkoG6yuIPlom/tnnPKqqGjttHNq57kqHbpqHOxw4LAtaqpu9Swu8WqvNXsrnnsr3qrwNSuvtfmsoKvwNjVt62vxNizw9rCyZPuuIS3yt3rv5Lvvo3twZDexqfpxZ7wxJX5w4jgyq290ufQ067iy6zyyJjzypzG1uLvzqDT09PX09H0z6LM2vHI3vL22rHU5Prt477R5vvW5Pr34LjW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/v25cHc6Pvc6Pzc6fvc6fzd6fve6fve6vvf6/zq6eng6/zg7Pzh7Pvh7Pz66sXi7fzi7vfj7fzj7v3l7v3w7ezy8fTt8/3s9Pzr9f7t9P7u9P3v9P3v9f7w9f7w9v3x9v39+Nfy9/3y9/7z9/70+P7///9mUkupYZeaAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMABojkvezAAAAHXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBUaGUgR0lNUO9kJW4AAAECSURBVBjTY2BgYMhLjg0L8nG30tdngIDsXbt2bt+yaYP+bqhI5q4dWzdvXL9GfzdUJHV1TnBc09p1QAGIiIOBiW5FvN8yfRBgYFhlFu3MIxNobujfsnL5UqCApoewl1OCCwuHZ/2KpYv0GWIUuPmj0gqKS2qL7BcvXqDFoKRnHZGeVd6QGFkmt3DeXDUGUUsuMVtW03xBDaOAuXNmKTOo2uyxCE+q7KguZW6dPX2KPIMbm7R3XduSbTWMuTOnT5kkxTBfvdGxuXtaKFPGjOmTJ/RJgBymbefLLlI1ferkif09QiCBkBTXws7pUyZO6O1q5wUJGOvoqKkoykqKC/BxcgIAmKJUjBV48xsAAAAASUVORK5CYII=); }\n"
"  .jpeg { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAACN1BMVEUJAQB5YFd1Y110Zmi5WESEbWSiZkeQb1SvaUtVgMiqbExXgcGFeXBZgcZYgsWSdXddhMRLpEpehMSCe5Z8gYi1b1Okd15hiMNiicNlisJni8GHhoWwe1ZpjMGLh45sjsC3eGtujr+4emRykL9ykb+5fmR2k72SjoyCnWq5gXB6lbuLj7B7lbyti2vOf1p+l7rEhGfMgly1jXSCmrytkIKtkX6/injifWChlZnTh1+Fnb7SiF3TiWDFkGnCkmmdnKOrlp2lmZ7PkGLDk3PYjGSTobC4loeLosKWn7vFlXHYkGWunZuRp8bKnHC7no+xopubq7vil2mXrMnMoJCbq97Mp4Kdsc3Pp4HkoG6yuIPlom/tnnPKqqGjttHNq57kqHbpqHOxw4LAtaqpu9Swu8WqvNXsrnnsr3qrwNSuvtfmsoKvwNjVt62vxNizw9rCyZPuuIS3yt3rv5Lvvo3twZDexqfpxZ7wxJX5w4jgyq290ufQ067iy6zyyJjzypzG1uLvzqDT09PX09H0z6LM2vHI3vL22rHU5Prt477R5vvW5Pr34LjW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/v25cHc6Pvc6Pzc6fvc6fzd6fve6fve6vvf6/zq6eng6/zg7Pzh7Pvh7Pz66sXi7fzi7vfj7fzj7v3l7v3w7ezy8fTt8/3s9Pzr9f7t9P7u9P3v9P3v9f7w9f7w9v3x9v39+Nfy9/3y9/7z9/70+P7///9mUkupYZeaAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhALOwzuu7tfAAAAHXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBUaGUgR0lNUO9kJW4AAAECSURBVBjTY2BgYMhLjg0L8nG30tdngIDsXbt2bt+yaYP+bqhI5q4dWzdvXL9GfzdUJHV1TnBc09p1QAGIiIOBiW5FvN8yfRBgYFhlFu3MIxNobujfsnL5UqCApoewl1OCCwuHZ/2KpYv0GWIUuPmj0gqKS2qL7BcvXqDFoKRnHZGeVd6QGFkmt3DeXDUGUUsuMVtW03xBDaOAuXNmKTOo2uyxCE+q7KguZW6dPX2KPIMbm7R3XduSbTWMuTOnT5kkxTBfvdGxuXtaKFPGjOmTJ/RJgBymbefLLlI1ferkif09QiCBkBTXws7pUyZO6O1q5wUJGOvoqKkoykqKC/BxcgIAmKJUjBV48xsAAAAASUVORK5CYII=); }\n"
"  .png { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAACN1BMVEUJAQB5YFd1Y110Zmi5WESEbWSiZkeQb1SvaUtVgMiqbExXgcGFeXBZgcZYgsWSdXddhMRLpEpehMSCe5Z8gYi1b1Okd15hiMNiicNlisJni8GHhoWwe1ZpjMGLh45sjsC3eGtujr+4emRykL9ykb+5fmR2k72SjoyCnWq5gXB6lbuLj7B7lbyti2vOf1p+l7rEhGfMgly1jXSCmrytkIKtkX6/injifWChlZnTh1+Fnb7SiF3TiWDFkGnCkmmdnKOrlp2lmZ7PkGLDk3PYjGSTobC4loeLosKWn7vFlXHYkGWunZuRp8bKnHC7no+xopubq7vil2mXrMnMoJCbq97Mp4Kdsc3Pp4HkoG6yuIPlom/tnnPKqqGjttHNq57kqHbpqHOxw4LAtaqpu9Swu8WqvNXsrnnsr3qrwNSuvtfmsoKvwNjVt62vxNizw9rCyZPuuIS3yt3rv5Lvvo3twZDexqfpxZ7wxJX5w4jgyq290ufQ067iy6zyyJjzypzG1uLvzqDT09PX09H0z6LM2vHI3vL22rHU5Prt477R5vvW5Pr34LjW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/v25cHc6Pvc6Pzc6fvc6fzd6fve6fve6vvf6/zq6eng6/zg7Pzh7Pvh7Pz66sXi7fzi7vfj7fzj7v3l7v3w7ezy8fTt8/3s9Pzr9f7t9P7u9P3v9P3v9f7w9f7w9v3x9v39+Nfy9/3y9/7z9/70+P7///9mUkupYZeaAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMAhT2HLg2AAAAHXRFWHRDb21tZW50AENyZWF0ZWQgd2l0aCBUaGUgR0lNUO9kJW4AAAECSURBVBjTY2BgYMhLjg0L8nG30tdngIDsXbt2bt+yaYP+bqhI5q4dWzdvXL9GfzdUJHV1TnBc09p1QAGIiIOBiW5FvN8yfRBgYFhlFu3MIxNobujfsnL5UqCApoewl1OCCwuHZ/2KpYv0GWIUuPmj0gqKS2qL7BcvXqDFoKRnHZGeVd6QGFkmt3DeXDUGUUsuMVtW03xBDaOAuXNmKTOo2uyxCE+q7KguZW6dPX2KPIMbm7R3XduSbTWMuTOnT5kkxTBfvdGxuXtaKFPGjOmTJ/RJgBymbefLLlI1ferkif09QiCBkBTXws7pUyZO6O1q5wUJGOvoqKkoykqKC/BxcgIAmKJUjBV48xsAAAAASUVORK5CYII=); }\n"
/* audio */
"  .mp3 { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAB+1BMVEUgYmc7PkM6Q09CRk1ER01DSE5FSE1GSU5GSk9GSlBHSlBIS09IS1BQVFtQVVtTVltYXWVYXmVYXmZdXl9gYGBeYWZpaWlmbXhsbnFubm5xcXF3d3d5eXlVgMhZgcZYgsV+f4BdhMRehMSBgoNhiMOCg4ViicNlisJni8FpjMGIiIiJiYlsjsBujr+KjI+JjJN8j6lykL9ykb+MjpB2k72OkZd6lbt7lbyQkpWSkpJ+l7qVlZWWlpaCmryFmriFnb6ampqWm6OZm52IoMCLosKfn5+doKWgoKCeoaefoqajo6ORp8ORp8akp6uoqKiXrMmsrKylr7ydsc2zs7OjttGpu9S2ub2yuse9wMOzw9q4w9W4xNa7xdXCxMjFyMvFytHCy9fHy9DAzODJy8/IzNLEzt7F0N/F0OHI0d7H0uPJ0+HK0+HP2OTL2u7N2u7O2uzP2+3O3O/P3O3Q3O3Q3O7Q3O/R3e/S3u7T3u7Y3ejU3+/V3+7W3+3X3+7W4O/Y4O/Z4e7Z4e/T4/nV4/nd5PHZ5fnZ5vra5/vd6Prc6fze6vvf6vrf6/zg6/zg7Pzh7Pvh7Pzi7fzj7fzj7vzj7v3k7vzl7vvl7vzl7v3m7/zm7/3n8P3o8P3p8f3q8vzr8v3s8/3t8/3t9P7u9P3u9f7v9f7w9f7///8AAABpfOEAAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAGAUYv3clAAAA80lEQVQY02NgYGCIDA0J8vdxsbeyYoCAwOS42IiwpYutlkNFgv0cbFylFy20Wg4VCXBKSTSWWjAfKAAR8fJdtsRCbN5cKxAACTh4Ll6gJzJ3zswZkyaCBSzd4lVlhGfPmj5lYj9QwNpd2UNBS1J/5tTJEyb0mjFIa0uLV5pKODZMnTShr6fLBKTFu2XmtMbauu6q8o5OQyB/hYFoTj0PO1cWH29mmw6Qz8CUwJ/NFs5SXBQjVKgBErCr4c5gZOEsLROMblYDCThXc6WxpZe05kcJ5CqBBOwrOFM5CtrzWJmTmuRAttiam5sY6WqqqyjKy8oCAAsLR8hfZq/DAAAAAElFTkSuQmCC); }\n"
"  .ogg { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAB5lBMVEUioQAsMz0xOkY0PEdCRk1CR01ESE1ESU9FSVBGSlBIS09NUVlOU1lPVFtQVFtQVVtRVVtSVltTVltTVlxdXl9gYGBpaWlmbXhsbnFubm5xcXF3d3d5eXlVgMhZgcZYgsV+f4BdhMRehMSBgoNhiMOCg4ViicNlisJni8FpjMGIiIiJiYl3jKpsjsBujr+KjI+JjJNykL9ykb+MjpB2k72OkZd6lbt7lbyQkpWSkpJ+l7qEmLOVlZWWlpaCmryFnb6ampqWm6OZm52LosKfn5+doKWgoKCeoaefoqajo6ORp8aSqMSkp6uoqKiXrMmsrKylr7ydsc2zs7OjttGpu9S2ub29wMOzw9q4w9XCxMi9x9XFyMvFytHHy9DAzODJy8/IzNLCzt/CzuHE0ODF0OHG0eHH0uHI0uLK0t/K0+HK0+LK1OLK1OPN1uPP2OTL2e3M2e7O2u3R3e/S3u7Y3ejU3u7U3+/X4PDZ4e7Z4e/T4/na4u/Y5fnY5vra5/vd6Prc6fze6fre6vvf6/zg6/vg6/zg7Pzh7Pvh7Pzj7Pvi7fzj7fzj7vzj7v3k7vzl7vrl7vzl7v3m7/zm7/3n8P3o8P3p8f3q8vzr8v3s8/3t8/3t9P7u9P3u9f7v9f7w9f7///8AAAC0meRkAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAFzNQnf5zAAAA9ElEQVQY02NgYGAIDwkO9PNytreyYoCAgPjoyLDQeXOsFkBFgnwdbF2kZ8+yWgAV8XdKiDWWmjkDKAAR8fSZP9dCbPo0KxAACTh4zJmpLzpt6qSJ3V1gAUvXGFUZkSmTJ/R2tQMFbNyU3RW0JA0m9fV0dLSaMUhrS4uXmEo4VvR1d7Q1N5mAtHhPnVRd2dnVUlZc32DIwLBwoXV/nrBQeUsOP19RnS7DQgYG5lyhzKzSYv7klMJaDZAAYwZXpgBnKkcKL3uBGkiAKVswLYonhTcxgjtfCWSGTmM6G1tKTRIrS1yVHMgWO3NzEyM9TXUVRXlZWQCkWUeD3N25dQAAAABJRU5ErkJggg==); }\n"
/* archive */
"  .rar { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABI1BMVEVzcyI0QW50NDRqQTR3NGdBZzREZzQ3Z5o0dG40dJp3Zzt3dDRqdGd3dG5VgMhZgcZYgsVdhMRehMRhiMNiicNlisJni8FpjMFsjsBujr9ykL9ykb+amjR2k72dmjR6lbt7lbx+l7rQdJqCmrxnp9OFnb5BzaCLosKRp8aXrMmdsc2jttGpu9SuvtfNzTuvwNizw9rd2prQ2tPa2tPU5PrW5PrW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/vc6Pvc6fvc6fzd6fve6fve6vvf6vzf6/zg6/zg7Pzh7Pvh7Pzi7fzj7fzj7vzj7v3l7vzl7v3m7/zt8/3t9P7u9P3v9P3v9f7w9f7w9v3x9v3y9/3y9/7z9/70+P7///80NDuIux2OAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWCxgAFw9/8oL0AAAAzUlEQVQY02NgYGAw0NHW0tRQV1VUZIAA/bi42OjI8FDFeKiIblxMVERYSLBiPESEGQi45PSYmYACIBE430gRBBhYoIDZyDgo0NdHkUEJAgy5jBMC/H28FME6gECGOcHIz9vbQ56BIQEMuNmNjL093V1lGVQgwJDXOMHD1cVJioERAmQ4E4xcnR3sJOBaOIyMHR3sbMQY1CDAkMc4wcHWykKEgYENDGRYE+xtrS3NhEBuV/VwcwFqt7YyNzXhBwkoKyjISkuKiwoLCvDxAQAD/yfVlasf2AAAAABJRU5ErkJggg==); }\n"
"  .bz2 { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABUFBMVEVib0RlXBtmZjNpZ16DdjBVgMhZgcZYgsVdhMRehMRhiMOViDhiicNlisJni8FpjMGNiH9sjsBujr9ykL9ykb92k72dk1qjlUN6lbt7lbx+l7qZmWaCmryimHKunUqFnb6IoMClnpCLosK3p1ewpXiRp8aXrMmyqpadsc2jttG3tK3Junapu9TDu5+uvtevwNizw9rMzJncy33QyrfMzMzg0Y/kza3s0JzZ1szq2Lfp18Xq3avn2+TW5fro4trY5vvZ5vva5/ra5/vc6Pvc6fvc6fzd6fve6fve6vve6vzf6vvf6vzf6/zg6/vg6/zg7Pzh7Pvh7Pz27cDi7fzj7fzj7vzj7v3k7vzl7vzl7v3m7/z179Tm7/3o8P3p8f3r8v3s8/3t8/3t9P7u9P3v9P3v9f7w9v3y9/35+Ojz9/70+P75+fb//8z+/v7///+AAH9Y67yrAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhALOxT91yMJAAAA+UlEQVQY0y3OWVeCUBQF4INhk81l5C21hEajgZuEUTmQhmXUBRtpHkmL8//fuizbj9/ae60NALC1vrqytJjLEgLdbLbb31+fH28k/JcNSo3O++sLCbsyZtDApYfPTxy4rO0YBl1Qt1X1kUSB5U4Y+IpeNKcf7m5bHgHlx/V96uYz4v3NlXfBG7+BaeZlSRKvLxlzZkDx9/d0OSMlYi123rSToBSpXpClBArMsY9rkzBb2NU0SeRwZtet6jhAWtY0URAQG0eIOMKP9aRlMeZgA61KCYeiq/Mea56ghZUyHgxEkHVOsY5YLvFJbwRzqVRyamJ0eLC/Lx7/A90eM8IF0VWrAAAAAElFTkSuQmCC); }\n"
"  .zip { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAAB6VBMVEUgICAAAAgAAAoAABsAAB8AAC4AAkUGEQYAGEAEHhsELDQANhcPLjsMLkUOLz0ML0UANE0AM1UANmgwLR0gNTInOkYAQJNhKwAOR2MATYQsQ0khQI3ZAABaRwCOLgAMXnIAbV3/AAAuXmH/CQ07XHA9XXA1VrBEXWIAioL/Iwr/JSr/Ly+bX1och+b/NzYsjdxVgMj9TCNZgcZYgsX/UwBdhMRehMRhiMNiicNlisJni8FpjMFsjsBujr//XjdykL9ykb//Yk52k716lbt7lbx+l7qCmrz/anGFnb6IoMCLosKMosKPpcSRp8b4nACYqc6XrMmdsc2nsMv/qwCjttH7rjepu9T/rEauvtf/vQCvwNi5v9qzw9r/qLG60+DbzcPsxc7xybD/2jz/zoj/xcX/9RXU5PrW5Pr/36nW5frW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/vc6Pvc6Pzc6fvc6fzd6fvd6fze6fve6vve6vzf6vvf6vzf6/zg6/vg6/zh6/zg7Pzh7Pvh7Pzi7fzj7fzj7vzj7v3k7vzl7vzl7v3/9Jrm7/zm7/3o8P3/+JXp8f3r8v3s8/3t8/3t9P7u9P3v9P3v9f7w9f7w9v3x9v3y9/3y9/7z9/70+P7//93///8AAAD51qJcAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhALNxPPBvmmAAAA+klEQVQY02NgYGCICQsJDPD18nB1ZYCAqPnz582ZNWOa60KoSMT8ubNnTp86xXUhVGQhi5CCgPTkSUABoIhmuOEiRjYuKV2Jia4gwKAc6+5oZ6KYosTT39vd1urKoL1w4YIJqbL+qrx9XR2tza4MWgszkkP9xINU+DvbW1oanBn0FvYkBUfKRavztbU01VU5MciwCy/iFOEW1RdsaaiqLLMHWZvAKqYhr9ZSX1VeXGgDEvBJjNdhbqyrLi0uzLcCCXjHLeKQZFpUUlyQm20BEvBsbamrqSguKsjLyTQDCXg01FYCteflZqWnGYEE3FxcnBxsrS3NTY0NDAC5lU3D988UOQAAAABJRU5ErkJggg==); }\n"
"  .gz { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABTVBMVEVsYQllXBtmZjNpZ16DdjBVgMhZgcZYgsVdhMRehMRhiMOViDhiicNlisJni8GNiH9sjsBykL9ykb92k72dk1qjlUN6lbt7lbx+l7qZmWaCmryimHKunUqFnb6IoMClnpCLosK3p1ewpXiRp8aXrMmyqpadsc2jttG3tK3Junapu9TDu5+uvtevwNizw9rMzJncy33QyrfMzMzg0Y/kza3s0JzZ1szq2Lfp18Xq3avn2+TW5fro4trY5vvZ5vva5/ra5/vc6Pvc6fvc6fzd6fvd6fze6fve6vve6vzf6vvf6vzf6/zg6/vg6/zg7Pzh7Pvh7Pz27cDi7fzj7fzj7vzj7v3k7vzl7vzl7v3m7/z179Tm7/3o8P3p8f3r8v3s8/3t8/3t9P7u9P3v9P3v9f7w9v3y9/35+Ojz9/70+P75+fb//8z+/v7///+AAH8mC3WdAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMAS2CNGP9AAAA+ElEQVQY0y3OaVeCQBgF4Bca22g3ckqtoNVoYZIwKlPbqIwasJX2lbR4///HhmP343PuPecCAKwuLczPzkznKYV2VprN76/Pjzca/8syY3br/fWFxm0ZtFnks53nJwFCFtdtm00Za4bxSJPAXCuOQt0qOaMPd7eNgIL+44ch8ws5cn9zFVyIxm/kOAVNVcn1JefeGOjh1qal5VRFbvDzupsGvcSsoqYqROKee7Q/DOPFDdNUiSJJZ+5BrToEkNVMk0iSjLiHiAPiWEdWI7KHx1irlLEvuToZ8PrJIWJlF7d7Esh7pyjmWBaTzgQmMpn0CGJ/b3dXKvUHxkgyA24INtQAAAAASUVORK5CYII=); }\n"
"  .tar { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABaFBMVEVkZ2VmZjNpZ16DdjBVgMhZgcZYgsVdhMRehMRhiMOViDhiicNlisJni8FpjMGNiH9sjsBujr9ykL9ykb92k72dk1qjlUN6lbt7lbx+l7qZmWaCmryimHKunUqFnb6IoMClnpCLosK3p1ewpXiRp8aXrMmyqpadsc2jttG3tK3Junapu9TDu5+uvtevwNizw9rMzJncy33QyrfMzMzg0Y/kza3s0JzZ1szq2Lfp18Xq3avn2+TU5PrW5PrW5fro4trW5fvX5fvY5frY5vvZ5vvZ5/va5/ra5/vb5/vc6Pvc6Pzc6fvc6fzd6fvd6fze6fve6vve6vzf6vvf6vzf6/zg6/vg6/zg7Pzh7Pvh7Pz27cDi7fzj7fzj7vzj7v3k7vzl7vzl7v3m7/z179Tm7/3o8P3p8f3r8v3s8/3t8/3t9P7u9P3v9P3v9f7w9v3y9/35+Ojz9/70+P75+fb//8z+/v7///9lXBs5uUpRAAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMARzT6mPHAAAA/ElEQVQY02NgYGDQ19ZQV1VRlJOUZIAAvaKiwvy83GzJMqiIrpGReXFOVqZkGUSE39yoIMrIPiMdKAAU0TQ2NzdS0DHQ0UmTBAEGteKygmRlUwsr0dSkxJhoSQblkqjkZKMoLVnmlIS46AigitICKystJTEx5vjYyMgwcQblZGtLUyVZMS7GmMjwkAARBmULI1MTJTEu5vLIsAB/HyEGKRMzQ0MxZq7y8tAAX093AQYGGSVDQ+bycsaQQG9Pd1deoMOYZJSYGcNCAr083ZwduUFOlY+ODAny8/Rwc3Gy4wAJyIUF+wO1uzg72NqwggSkJSREhAX5eDjZ2VhYAPTpNFDkSwh2AAAAAElFTkSuQmCC); }\n"
/* other */
"  .pdf { background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAMAAAAoLQ9TAAABPlBMVEVtCQJjAACMAAiUAABzGBitAAi1AADGAADOCAicISHOEBDnCBDWGBj3EBD3GBjOKTGcQkK9OTn3KSneOTm9SkrvOTn/MTnOSkq9UlreSkrvQkrnSkpVgMjvSkpZgcZYgsVdhMRehMTvUlLvUlphiMNiicPnWlplisLnWmNni8HWY2tpjMFsjsBujr9ykL9ykb92k716lbt7lbx+l7rnc3OCmryFnb6IoMD/c3Pve3uLosKMosLehISPpcSRp8aSqMbnjIyVq8iXrMmZrcqcsMydsc2jttHvnJypu9SsvtavwNizw9rvra3etb3v1tbS4vrU4/rW5PrX5fvZ5vvb5/vc6Pve6fvg6vzh7Pzj7fzl7vzn7/fm7/zo8P3q8f3r8v3t8/3v9P3w9v7y9/70+P71+f73+v75+/////9aAACwyNz9AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxIAAAsSAdLdfvwAAAAHdElNRQfWBhAMAijZc8SxAAAA0klEQVQY02NgYGDw9nBzdbKzMjM2ZoAAr/S01JTkpETjDKiIJ4SfYJwBEpEQERTXUFNWk+KPBwoARSxkxYT4+Hj5uFnjjEGAQZSHi4udg52VjSk2JioywpiBmTGTRQAIOAVA/HCwsS6JCfFxkmB+mCFIwBnI9xN2B/FDDUACjvFxsZbRDj5AfogeSMA+LtZP2sdHUlrJJlgHJGAbG6PmC1LvqxWkCRKwjtFwAPFDgoMCVUEC5iYmMH6AIkjALBLO95cDCZgaGRno62qrqyjIy8gAAGduNQKHw8HXAAAAAElFTkSuQmCC); }\n"
"/*]]>*/\n"
"  </style>\n"
"  <meta http-equiv='Content-Type' content='application/xhtml+xml' />\n\n"
/* directory we are indexing */
"  <title>Index of %s</title>\n"
"</head>\n\n"
"<body>\n"
"	<div id='content'>\n"
/* directory we are indexing */
"		<h2 style='float: left'>Index of %s</h2>\n"
"       <span style='font-style: italic; float:right'>" PROGRAM " " VERSION "</span>\n"
"       <div class='clear'></div>\n"
"		<div id='top'>\n"
"			<div class='ico'></div>\n"
"			<div class='filename'>Filename</div>\n"
"			<div class='modified'>Last Modified</div>\n"
"			<div class='size'>Size</div>\n"
"		</div>\n"
"		<div class='bar'></div>\n\n"
/* dir_entry listing goes here */
"%s"
"	</div>\n"
"</body>\n"
"</html>\n";

static const char *dir_entry =
"		<div>\n"
"			<div class='ico %s'></div>\n" /* base64 icon */
"			<div class='filename'><a href='%s'>%.30s</a></div>\n" /* file name, file name */
"			<div class='modified'>%s</div>\n" /* date last modified */
"			<div class='size'>%s %s</div>\n" /* size of file and suffix */
"		</div>\n"
"		<div class='clear'></div>\n\n";

static const struct {
	char *ext;
} icons[] = {
	{"html"},
	{"htm"},
	{"css"},
	{"js"},
	{"xhtml"},
	{"gif"},
	{"jpg"},
	{"jpeg"},
	{"png"},
	{"mp3"},
	{"ogg"},
	{"rar"},
	{"bz2"},
	{"zip"},
	{"gz"},
	{"tar"},
	{"pdf"},
	{0}
};

const int make_dir_list(connection *ret) {
	char buf[8 * KILOBYTE] = {0};
	struct stat stattmp;
	struct dirent **filelist;
	int filecount = 0;
	char dir[8 * KILOBYTE] = {0};
	char **entries = 0;

	/* FIX THIS LATER */
	char *filenamebuf;
	char iconbuf[10] = {0};
	char timestampbuf[256] = {0};
	char sizebuf[50];
	float bytes = 0;
	const char *suffix = 0;
	struct tm timetmp;
	const char *timestr = "%d-%b-%Y %H:%M";

	memset(&timetmp, 0, sizeof(struct tm));

	int entrieslen = 0;

	/* so, the basic idea is as follows:
	 * 1. scan the directory
	 * 2. stat the file for infos
	 * 3. grab the filename
	 * 4. get an icon based on filename/type
	 * 5. get the size
	 * 6. get the last modified timestamp
	 * 7. pop it in dir_entry
	 * 8. put those in dir_page
	 * 9. return all that shit */

	// always start relative
	strcpy(dir, "./");
	url_decode(buf, ret->req.resource+1);
	strcat(dir, buf);

	// filter out ".." if we are in /
	if( strncmp(dir, "./", 3) ) {
		filecount = scandir(dir, &filelist, yesparent, versionsort);
	} else {
		filecount = scandir(dir, &filelist, noparent, versionsort);
	}

	if(filecount >= 0) {
		int cnt;
		entries = malloc(filecount * sizeof(*entries));
		memset(entries, 0, sizeof(entries));

		for(cnt = 0; cnt < filecount; ++cnt) {
			char statbuf[1024];
			strcpy(statbuf, dir);
			strcat(statbuf, filelist[cnt]->d_name);
			stat(statbuf, &stattmp);

			// get file name
			filenamebuf = strdup(filelist[cnt]->d_name);
			filenamebuf = realloc(filenamebuf, strlen(filenamebuf) + 5);

			if( S_ISREG(stattmp.st_mode) ) {
				strcpy(iconbuf, icon_from_fname(filenamebuf) );
				// set proper suffix
				suffix = get_suffx_from_size(stattmp.st_size, &bytes);
				if(bytes==floor(bytes)) {
					sprintf(sizebuf, "%.0f", bytes);
				} else {
					sprintf(sizebuf, "%.1f", bytes);
				}
			} else if(S_ISDIR(stattmp.st_mode)) {
				strcpy(iconbuf, "folder");
				strcpy(sizebuf, "-");
				strcat(filenamebuf, "/");
				suffix = "";
			} else {
				/* isn't a regular file or folder... wtf is it? */
				strcpy(iconbuf, "file");
				strcpy(sizebuf, "-");
			}

			/* 6. gimme a timestamp */
			gmtime_r(&stattmp.st_mtime, &timetmp);
			strftime(timestampbuf, 256, timestr, &timetmp);

			/* 7. put in buffer, order = icon, filename, time modified, size */
			/* put all the directory data into the div tags */
			int len;
			if( strcmp(filenamebuf, "../") == 0) {
				len = asprintf(&entries[cnt], dir_entry, "arrow", filenamebuf, "Parent Directory", "", sizebuf, "");
			} else {
				len = asprintf(&entries[cnt], dir_entry, iconbuf, filenamebuf, filenamebuf, timestampbuf, sizebuf, suffix);
			}
			entrieslen += len + 1;

			/* free the linked list and file name buffer */
			free(filelist[cnt]);
			free(filenamebuf);

		}

		char *entriestotal = malloc(entrieslen * sizeof(*entriestotal));
		/* danke BhaaL */
		memset(entriestotal, 0, entrieslen);

		for(cnt = 0; cnt < filecount; ++cnt) {
			strcat(entriestotal, entries[cnt]);
			free(entries[cnt]);
		}
		free(entries);
		free(filelist);

		ret->response.content_size = asprintf(&ret->response.data, dir_page, dir+1, dir+1, entriestotal);
		if(ret->response.content_size <= 0) {
			/* asprintf went bad */
			BARK("asprintf(): %s\n", strerror(errno));
			ret->response.status = HTTP_INTERNAL_ERROR;
			error_code_to_data( &(ret->response) );
			return -1;
		}

		free(entriestotal);

		/* our directory listings are XHTML 1.0 Strict compliant, damnit :(
		* sigh, send as text/html to compensate for IE and other crap browsers..
		* even though that isn't ideal and is read as tag soup.. */
		strcpy(ret->response.mimetype, "text/html");

		return 0;
	} else {
		BARK("scandir failed: %s\n", strerror(errno));
		ret->response.status = HTTP_NOT_FOUND;
		error_code_to_data( &(ret->response) );
		return -1;
	}

	// something odd happened
	return -1;
}

const char *icon_from_fname(const char *filename) {
	int i, len_ico, len = strlen(filename);

	for(i=0;icons[i].ext != 0;i++) {
		len_ico = strlen(icons[i].ext);
		if( strncmp(&filename[len-len_ico], icons[i].ext, len_ico) == 0) {
			return icons[i].ext;
		}
	}

	// no icon. send a blank one.
	return "file";
}

const char *get_suffx_from_size(unsigned long bytes, float *floaty) {
	static char const *suffix[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", 0};
	int i;
	*floaty = bytes;

	for(i = 0; bytes > 1024; ++i) {
		bytes /= 1024;
		*floaty /= 1024;
		}
	if(suffix[i] == 0)
		return suffix[6];

	return suffix[i];
}

int exist_index(connection *ret) {

	DIR *dp;
	struct dirent *ep;
	char dir[FILENAME_MAX] = {0};
	char buf[FILENAME_MAX] = {0};
	strcpy(dir, "./");
	url_decode(buf, ret->req.resource+1);
	strcat(dir, buf);

	dp = opendir (dir);
	if (dp != NULL)
	{
		while ( (ep = readdir (dp))  ) {
			if( strcmp(ep->d_name, "index.xhtml") == 0) {
				strcpy(buf, dir);
				strcat(buf, ep->d_name);
				free(ret->req.resource);
				ret->req.resource = strdup(buf+1);
				closedir (dp);
				return 1;
			} else if( strcmp(ep->d_name, "index.html") == 0) {
				strcpy(buf, dir);
				strcat(buf, ep->d_name);
				free(ret->req.resource);
				ret->req.resource = strdup(buf+1);
				closedir (dp);
				return 1;
			} else if( strcmp(ep->d_name, "index.htm") == 0) {
				strcpy(buf, dir);
				strcat(buf, ep->d_name);
				free(ret->req.resource);
				ret->req.resource = strdup(buf+1);
				closedir (dp);
				return 1;
			}
		}

		closedir (dp);
	} else {
		BARK("failed reading directory %s\n", dir);
	}

	return 0;
}

int noparent(const struct dirent *entry) {
	if( strncmp(entry->d_name, "..", 3) == 0) return 0;
	if( strncmp(entry->d_name, ".", 2) == 0) return 0;
	return 1;
}

int yesparent(const struct dirent *entry) {
	if( strncmp(entry->d_name, ".", 3) == 0) return 0;
	return 1;
}
