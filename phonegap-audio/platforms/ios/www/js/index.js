/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
 (function($, container) {
    "use strict";

    var fileTransfer;

    var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
    },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);

        $(container).on("click", "#play-audio",function (e) {
            player.initialize();
            e.preventDefault();
        })
        .on("click", "#pause-audio", function (e) {
            player.pause();
            e.preventDefault();
        })
        .on("click", "#resume-audio", function (e) {
            player.resume();
            e.preventDefault();
        })
        .on('change', '#player-bar', function(){
            player.onChangeTimer($(this).val() * 1000);
        })
        .on("click", "#stop-audio", function (e) {
            player.stop();
            e.preventDefault();
        })
        .on("click", "#download-audio", function (e) {
            var url = $("#url-download").text();
            app.executeDownloadFile(url);
            e.preventDefault();
        });
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        $("#app-initialization").hide();
        $("#player-content").hide();
        $("#resume-audio").hide();
        $("#pause-audio").hide();
        $("#stop-audio").hide();
    },
    executeDownloadFile: function (url){
        fileTransfer = new FileTransfer();
        $("#player-content").hide();

        fileTransfer.onprogress = function(result){
            var actual = (result.loaded/1024);
            var total = (result.total/1024);
            $("#download-status").html(actual.toFixed(2) + 'kb de ' + total.toFixed(2) + ' kb');
        };

        var uri = encodeURI(url);
        var fileName = url.split('/')[url.split('/').length - 1];

        console.log('filename:' + fileName);

        window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, function (fs) {
            var fullpath = cordova.file.dataDirectory  +"_audio/" + fileName;
            fileTransfer.download(
                uri,
                fullpath,
                function (entry) {
                    $("#player-content").show();
            }, function (error) {
                   console.log("download error: " + JSON.stringify(error));
            });
        });
    }
};



var player = {
    media: null,
    timer: null,
    initialize: function () {
        if(this.media != null){
            this.media.stop();
            this.media.release();
            this.media = null;
        }

        this.media = new Media('audio-sample/1028.mp3', this.onMediaSuccess, this.onMediaError, this.onMediaStatus);

        $("#audio-name").html('1028.mp3');

        player.timer = setInterval(function(){
          player.media.getCurrentPosition(function(position){
            if(position > -1){
              var time = player.secondsToTime(position);
              document.getElementById('player-current-time').innerHTML = time.m + ":" + time.s;
              document.getElementById('player-bar').value = position;

              var audioDuration = player.media.getDuration();
              var time = player.secondsToTime(audioDuration);
              document.getElementById('player-duration-time').innerHTML = time.m + ":" + time.s;
              document.getElementById('player-bar').setAttribute('max', audioDuration);
          }
      });
      }, 999);

        this.media.play({ playAudioWhenScreenIsLocked : true });
    },
    onMediaSuccess: function () {
        console.log("media success");
    },
    onMediaError: function (error) {
        console.log("audio player error: " + JSON.stringify(error));
    },
    onMediaStatus: function (status) {
        switch(status){
            case 0:
            break;
            case 1:
            case 2:
            {
                $("#pause-audio").show();
                $("#resume-audio").hide();
                $("#stop-audio").show();
                break;
            }
            case 3:
            {
                $("#pause-audio").hide();
                $("#resume-audio").hide();
                $("#stop-audio").hide();
                break;
            }
            case 4:
            {
                $("#pause-audio").hide();
                $("#resume-audio").hide();
                $("#stop-audio").hide();
                break;
            }
            default:
            break;
        };
    },
    onChangeTimer: function(seconds){
        player.media.seekTo(seconds);
    },
    pause: function () {
        if (player.media) {
            player.media.pause();
            $("#resume-audio").show();
        }
    },
    resume: function () {
        if (player.media) {
            player.media.play();
            $("#resume-audio").hide();
            $("#pause-audio").hide();
        }
    },
    stop: function () {
        if (player.media) {
            player.media.stop();
        }
        clearInterval(player.timer);
        player.timer = null;
    },
    secondsToTime: function(sec){
      var divisor_for_minutes = sec % (60 * 60);
      var minutes = Math.floor(divisor_for_minutes / 60);

      var divisor_for_seconds = divisor_for_minutes % 60;
      var seconds = Math.ceil(divisor_for_seconds);

      if(minutes < 10) minutes = "0" + minutes;

      if(seconds < 10) seconds = "0" + seconds;

      var obj = {
          "m": minutes,
          "s": seconds
      };
      return obj;
  }
};


$(function () {
    app.initialize();
});

})(jQuery,document);