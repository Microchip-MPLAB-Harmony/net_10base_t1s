var myVariable = `
{"0": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "content": "This example shows how to configure the LAN867x to be used in a 10BASE-T1S Multi-Drop Ethernet network in either PLCA and CSMA/CD mode. It also shows how to r/w registers of the LAN867x during normal operation. Additionally, common Ethernet network functionalites are supported, e.g. ping and iperf. ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html#tcpip-client-using-the-lan867x-10base-t1s-ethernet-phy",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html#tcpip-client-using-the-lan867x-10base-t1s-ethernet-phy"
  },"1": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "Building The Application",
    "content": "The parent folder for all the MPLAB X IDE projects for this application is given below: . Application Path : net_10base_t1s\\\\apps\\tcpip_iperf_10base_t1s\\firmware . To build the application, refer to the table below and open the appropriate project file in MPLABX IDE. | Project Name | Description | . | tcpip_iperf_10base_t1s.X | TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY | . ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html#building-the-application",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html#building-the-application"
  },"2": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "Hardware setup",
    "content": ". | Hard used . | Sam E54 Curiosity Ultra | 10BASE-T1S PHY RMII Card Please contact 10BASE-T1S Support for more information. | . | Hardware setup . | Connect the DEBUG USB port on the board to the computer using a micro USB cable | Connect the 10BASE-T1S PHY RMII Card to another 10BASE-T1S node (e.g. a second instance of this setup) | . | . ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html#hardware-setup",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html#hardware-setup"
  },"3": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "Harmony 3 Setting for LAN867x",
    "content": "GMAC Settings . 10BASE-T1S is a 10 Mbps, half-duplex network. Therefore, the GMAC must be configured accordinegly to: . | 10 Mbps | Half Duplex | Auto Negotiation - Disabled | MDIX - Disabled | . PLCA Settings . 10BASE-T1S can be used in PLCA or CSMA/CD mode. When using PLCA, the parameters for Local Node ID, Node Count, Max Burst Count and Burst Timer must be configured. ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html#harmony-3-setting-for-lan867x",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html#harmony-3-setting-for-lan867x"
  },"4": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "Running the Application",
    "content": ". | Open a Terminal application (e.g. Tera term) on the computer | Connect to the Virtual COM port and configure the serial settings as follows: . | Baud : 115200 | Data : 8 Bits | Parity : None | Stop : 1 Bit | Flow Control : None | . | Build and Program the application using the MPLAB X IDE | See the messages in the console | . ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html#running-the-application",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html#running-the-application"
  },"5": {
    "doc": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "title": "TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY",
    "content": ". ",
    "url": "http://localhost:4000/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html",
    "relUrl": "/apps/tcpip_iperf_10base_t1s/readme.html"
  },"6": {
    "doc": "Example applications",
    "title": "Example applications",
    "content": "The following applications are provided to demonstrate the typical or interesting usage models of one or more 10BASE-T1S devices. ",
    "url": "http://localhost:4000/net_10base_t1s/apps/readme.html#example-applications",
    "relUrl": "/apps/readme.html#example-applications"
  },"7": {
    "doc": "Example applications",
    "title": "LAN867x Examples",
    "content": ". | TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY | . ",
    "url": "http://localhost:4000/net_10base_t1s/apps/readme.html#lan867x-examples",
    "relUrl": "/apps/readme.html#lan867x-examples"
  },"8": {
    "doc": "Example applications",
    "title": "Example applications",
    "content": ". ",
    "url": "http://localhost:4000/net_10base_t1s/apps/readme.html",
    "relUrl": "/apps/readme.html"
  },"9": {
    "doc": "Release notes",
    "title": "Release notes",
    "content": "![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png) ![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png) # Microchip MPLAB® Harmony 3 Release Notes ## 10BASE-T1S Release v1.0.0 ### New Features - **New device support** - This release introduces support for | Device | Description | ------ | ------ | LAN867x | 10BASE-T1S Ethernet PHY | - **Demo application support** - The following table provides a list of of new applications | Application | Platform | Description | ------ | ------ | ------ | tcpip_iperf_10base_t1s | SAM E54 | TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY | - **Development kit support** - The following table provides a list of supported development kits | Development Kits | ------ | [SAM E54 Curiosity Ultra Development Board](https://www.microchip.com/Developmenttools/ProductDetails/DM320210) | In addition to the listed development kits above, a 10BASE-T1S PHY RMII Card is required. Please contact [10BASE-T1S Support](10BASE-T1S-Info@microchip.com) for more information. ### Known Issues * No known issues ### Dependencies * [MPLAB® X IDE v5.45](https://www.microchip.com/mplab/mplab-x-ide) or later * [MPLAB® XC32 C/C++ Compiler v2.50](https://www.microchip.com/mplab/compilers) or later * [net](https://github.com/Microchip-MPLAB-Harmony/net/tree/v3.7.2) ",
    "url": "http://localhost:4000/net_10base_t1s/release_notes.html",
    "relUrl": "/release_notes.html"
  },"10": {
    "doc": "Harmony 3 10BASE-T1S Package",
    "title": "Harmony 3 10BASE-T1S Package",
    "content": "![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png) ![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png) # MPLAB® Harmony 3 10BASE-T1S MPLAB® Harmony 3 is an extension of the MPLAB® ecosystem for creating embedded firmware solutions for Microchip 32-bit SAM and PIC® microcontroller and microprocessor devices. Refer to the following links for more information. - [Microchip 32-bit MCUs](https://www.microchip.com/design-centers/32-bit) - [Microchip 32-bit MPUs](https://www.microchip.com/design-centers/32-bit-mpus) - [Microchip MPLAB® X IDE](https://www.microchip.com/mplab/mplab-x-ide) - [Microchip MPLAB® Harmony](https://www.microchip.com/mplab/mplab-harmony) - [Microchip MPLAB® Harmony Pages](https://microchip-mplab-harmony.github.io/) This repository contains the MPLAB® Harmony 3 10BASE-T1S solutions and example applications. Quickly incorporate connectivity to your designs with 10BASE-T1S ICs, modules, software and development kits that make connecting effortless for your customers. Our comprehensive 10BASE-T1S portfolio has the technology to meet your range, data rate, interoperability, frequency and topology needs. Refer to the following links for release notes, training materials, and interface reference information. - [Release Notes](/net_10base_t1s/release_notes.html) - [MPLAB® Harmony License](/net_10base_t1s/mplab_harmony_license.html) - [10BASE-T1S Support](10BASE-T1S-Info@microchip.com) # Contents Summary | Folder | Description | --- | --- | apps | Example applications for 10BASE-T1S devices | config | 10BASE-T1S module configuration file | docs | 10BASE-T1S package documentation | drivers | Driver/Firmware for 10BASE-T1S devices | ## Code Examples The following applications are provided to demonstrate the typical or interesting usage models of one or more peripheral libraries. | Name | Status | --- | --- |[TCP/IP Client using the LAN867x 10BASE-T1S Ethernet PHY](/net_10base_t1s/apps/tcpip_iperf_10base_t1s/readme.html) | ![app-production](https://img.shields.io/badge/application-production-brightgreen?style=plastic) | Note: The following link guides through the steps to create a new TCP/IP application using MPLAB® Harmony 3 modules. [Create TCP/IP New Application Example](https://github.com/Microchip-MPLAB-Harmony/net/wiki/Create-your-first-tcpip-application) ____ [![License](https://img.shields.io/badge/license-Harmony%20license-orange.svg)](https://github.com/Microchip-MPLAB-Harmony/net_10base_t1s/blob/master/mplab_harmony_license.md) [![Latest release](https://img.shields.io/github/release/Microchip-MPLAB-Harmony/net_10base_t1s.svg)](https://github.com/Microchip-MPLAB-Harmony/net_10base_t1s/releases/latest) [![Latest release date](https://img.shields.io/github/release-date/Microchip-MPLAB-Harmony/net_10base_t1s.svg)](https://github.com/Microchip-MPLAB-Harmony/net_10base_t1s/releases/latest) [![Commit activity](https://img.shields.io/github/commit-activity/y/Microchip-MPLAB-Harmony/net_10base_t1s.svg)](https://github.com/Microchip-MPLAB-Harmony/net_10base_t1s/graphs/commit-activity) [![Contributors](https://img.shields.io/github/contributors-anon/Microchip-MPLAB-Harmony/net_10base_t1s.svg)]() ____ [![Follow us on Youtube](https://img.shields.io/badge/Youtube-Follow%20us%20on%20Youtube-red.svg)](https://www.youtube.com/user/MicrochipTechnology) [![Follow us on LinkedIn](https://img.shields.io/badge/LinkedIn-Follow%20us%20on%20LinkedIn-blue.svg)](https://www.linkedin.com/company/microchip-technology) [![Follow us on Facebook](https://img.shields.io/badge/Facebook-Follow%20us%20on%20Facebook-blue.svg)](https://www.facebook.com/microchiptechnology/) [![Follow us on Twitter](https://img.shields.io/twitter/follow/MicrochipTech.svg?style=social)](https://twitter.com/MicrochipTech) [![](https://img.shields.io/github/stars/Microchip-MPLAB-Harmony/net_10base_t1s.svg?style=social)]() [![](https://img.shields.io/github/watchers/Microchip-MPLAB-Harmony/net_10base_t1s.svg?style=social)]() ",
    "url": "http://localhost:4000/net_10base_t1s/",
    "relUrl": "/"
  }
}
`;
var data_for_search

var repo_name = "net_10base_t1s";
var doc_folder_name = "docs";
var localhost_path = "http://localhost:4000/";
var home_index_string = "10BASE-T1S";

(function (jtd, undefined) {

// Event handling

jtd.addEvent = function(el, type, handler) {
  if (el.attachEvent) el.attachEvent('on'+type, handler); else el.addEventListener(type, handler);
}
jtd.removeEvent = function(el, type, handler) {
  if (el.detachEvent) el.detachEvent('on'+type, handler); else el.removeEventListener(type, handler);
}
jtd.onReady = function(ready) {
  // in case the document is already rendered
  if (document.readyState!='loading') ready();
  // modern browsers
  else if (document.addEventListener) document.addEventListener('DOMContentLoaded', ready);
  // IE <= 8
  else document.attachEvent('onreadystatechange', function(){
      if (document.readyState=='complete') ready();
  });
}

// Show/hide mobile menu

function initNav() {
  jtd.addEvent(document, 'click', function(e){
    var target = e.target;
    while (target && !(target.classList && target.classList.contains('nav-list-expander'))) {
      target = target.parentNode;
    }
    if (target) {
      e.preventDefault();
      target.parentNode.classList.toggle('active');
    }
  });

  const siteNav = document.getElementById('site-nav');
  const mainHeader = document.getElementById('main-header');
  const menuButton = document.getElementById('menu-button');

  jtd.addEvent(menuButton, 'click', function(e){
    e.preventDefault();

    if (menuButton.classList.toggle('nav-open')) {
      siteNav.classList.add('nav-open');
      mainHeader.classList.add('nav-open');
    } else {
      siteNav.classList.remove('nav-open');
      mainHeader.classList.remove('nav-open');
    }
  });
}
// Site search

function initSearch() {

    data_for_search = JSON.parse(myVariable);
    lunr.tokenizer.separator = /[\s/]+/

    var index = lunr(function () {
        this.ref('id');
        this.field('title', { boost: 200 });
        this.field('content', { boost: 2 });
        this.field('url');
        this.metadataWhitelist = ['position']

        var location = document.location.pathname;
        var path = location.substring(0, location.lastIndexOf("/"));
        var directoryName = path.substring(path.lastIndexOf("/")+1);

        var cur_path_from_repo = path.substring(path.lastIndexOf(repo_name));

        // Decrement depth by 2 as HTML files are placed in repo_name/doc_folder_name
        var cur_depth_from_doc_folder = (cur_path_from_repo.split("/").length - 2);

        var rel_path_to_doc_folder = "";

        if (cur_depth_from_doc_folder == 0) {
            rel_path_to_doc_folder = "./"
        }
        else {
            for (var i = 0; i < cur_depth_from_doc_folder; i++)
            {
                rel_path_to_doc_folder = rel_path_to_doc_folder + "../"
            }
        }

        for (var i in data_for_search) {

            data_for_search[i].url = data_for_search[i].url.replace(localhost_path + repo_name, rel_path_to_doc_folder);

            if (data_for_search[i].title == home_index_string)
            {
                data_for_search[i].url = data_for_search[i].url + "index.html"
            }

            this.add({
                id: i,
                title: data_for_search[i].title,
                content: data_for_search[i].content,
                url: data_for_search[i].url
            });
        }
    });

    searchLoaded(index, data_for_search);
}function searchLoaded(index, docs) {
  var index = index;
  var docs = docs;
  var searchInput = document.getElementById('search-input');
  var searchResults = document.getElementById('search-results');
  var mainHeader = document.getElementById('main-header');
  var currentInput;
  var currentSearchIndex = 0;

  function showSearch() {
    document.documentElement.classList.add('search-active');
  }

  function hideSearch() {
    document.documentElement.classList.remove('search-active');
  }

  function update() {
    currentSearchIndex++;

    var input = searchInput.value;
    if (input === '') {
      hideSearch();
    } else {
      showSearch();
      // scroll search input into view, workaround for iOS Safari
      window.scroll(0, -1);
      setTimeout(function(){ window.scroll(0, 0); }, 0);
    }
    if (input === currentInput) {
      return;
    }
    currentInput = input;
    searchResults.innerHTML = '';
    if (input === '') {
      return;
    }

    var results = index.query(function (query) {
      var tokens = lunr.tokenizer(input)
      query.term(tokens, {
        boost: 10
      });
      query.term(tokens, {
        wildcard: lunr.Query.wildcard.TRAILING
      });
    });

    if ((results.length == 0) && (input.length > 2)) {
      var tokens = lunr.tokenizer(input).filter(function(token, i) {
        return token.str.length < 20;
      })
      if (tokens.length > 0) {
        results = index.query(function (query) {
          query.term(tokens, {
            editDistance: Math.round(Math.sqrt(input.length / 2 - 1))
          });
        });
      }
    }

    if (results.length == 0) {
      var noResultsDiv = document.createElement('div');
      noResultsDiv.classList.add('search-no-result');
      noResultsDiv.innerText = 'No results found';
      searchResults.appendChild(noResultsDiv);

    } else {
      var resultsList = document.createElement('ul');
      resultsList.classList.add('search-results-list');
      searchResults.appendChild(resultsList);

      addResults(resultsList, results, 0, 10, 100, currentSearchIndex);
    }

    function addResults(resultsList, results, start, batchSize, batchMillis, searchIndex) {
      if (searchIndex != currentSearchIndex) {
        return;
      }
      for (var i = start; i < (start + batchSize); i++) {
        if (i == results.length) {
          return;
        }
        addResult(resultsList, results[i]);
      }
      setTimeout(function() {
        addResults(resultsList, results, start + batchSize, batchSize, batchMillis, searchIndex);
      }, batchMillis);
    }

    function addResult(resultsList, result) {
      var doc = docs[result.ref];

      var resultsListItem = document.createElement('li');
      resultsListItem.classList.add('search-results-list-item');
      resultsList.appendChild(resultsListItem);

      var resultLink = document.createElement('a');
      resultLink.classList.add('search-result');
      resultLink.setAttribute('href', doc.url);
      resultsListItem.appendChild(resultLink);

      var resultTitle = document.createElement('div');
      resultTitle.classList.add('search-result-title');
      resultLink.appendChild(resultTitle);

      var resultDoc = document.createElement('div');
      resultDoc.classList.add('search-result-doc');
      resultDoc.innerHTML = '<svg viewBox="0 0 24 24" class="search-result-icon"><use xlink:href="#svg-doc"></use></svg>';
      resultTitle.appendChild(resultDoc);

      var resultDocTitle = document.createElement('div');
      resultDocTitle.classList.add('search-result-doc-title');
      resultDocTitle.innerHTML = doc.doc;
      resultDoc.appendChild(resultDocTitle);
      var resultDocOrSection = resultDocTitle;

      if (doc.doc != doc.title) {
        resultDoc.classList.add('search-result-doc-parent');
        var resultSection = document.createElement('div');
        resultSection.classList.add('search-result-section');
        resultSection.innerHTML = doc.title;
        resultTitle.appendChild(resultSection);
        resultDocOrSection = resultSection;
      }

      var metadata = result.matchData.metadata;
      var titlePositions = [];
      var contentPositions = [];
      for (var j in metadata) {
        var meta = metadata[j];
        if (meta.title) {
          var positions = meta.title.position;
          for (var k in positions) {
            titlePositions.push(positions[k]);
          }
        }
        if (meta.content) {
          var positions = meta.content.position;
          for (var k in positions) {
            var position = positions[k];
            var previewStart = position[0];
            var previewEnd = position[0] + position[1];
            var ellipsesBefore = true;
            var ellipsesAfter = true;
            for (var k = 0; k < 5; k++) {
              var nextSpace = doc.content.lastIndexOf(' ', previewStart - 2);
              var nextDot = doc.content.lastIndexOf('. ', previewStart - 2);
              if ((nextDot >= 0) && (nextDot > nextSpace)) {
                previewStart = nextDot + 1;
                ellipsesBefore = false;
                break;
              }
              if (nextSpace < 0) {
                previewStart = 0;
                ellipsesBefore = false;
                break;
              }
              previewStart = nextSpace + 1;
            }
            for (var k = 0; k < 10; k++) {
              var nextSpace = doc.content.indexOf(' ', previewEnd + 1);
              var nextDot = doc.content.indexOf('. ', previewEnd + 1);
              if ((nextDot >= 0) && (nextDot < nextSpace)) {
                previewEnd = nextDot;
                ellipsesAfter = false;
                break;
              }
              if (nextSpace < 0) {
                previewEnd = doc.content.length;
                ellipsesAfter = false;
                break;
              }
              previewEnd = nextSpace;
            }
            contentPositions.push({
              highlight: position,
              previewStart: previewStart, previewEnd: previewEnd,
              ellipsesBefore: ellipsesBefore, ellipsesAfter: ellipsesAfter
            });
          }
        }
      }

      if (titlePositions.length > 0) {
        titlePositions.sort(function(p1, p2){ return p1[0] - p2[0] });
        resultDocOrSection.innerHTML = '';
        addHighlightedText(resultDocOrSection, doc.title, 0, doc.title.length, titlePositions);
      }

      if (contentPositions.length > 0) {
        contentPositions.sort(function(p1, p2){ return p1.highlight[0] - p2.highlight[0] });
        var contentPosition = contentPositions[0];
        var previewPosition = {
          highlight: [contentPosition.highlight],
          previewStart: contentPosition.previewStart, previewEnd: contentPosition.previewEnd,
          ellipsesBefore: contentPosition.ellipsesBefore, ellipsesAfter: contentPosition.ellipsesAfter
        };
        var previewPositions = [previewPosition];
        for (var j = 1; j < contentPositions.length; j++) {
          contentPosition = contentPositions[j];
          if (previewPosition.previewEnd < contentPosition.previewStart) {
            previewPosition = {
              highlight: [contentPosition.highlight],
              previewStart: contentPosition.previewStart, previewEnd: contentPosition.previewEnd,
              ellipsesBefore: contentPosition.ellipsesBefore, ellipsesAfter: contentPosition.ellipsesAfter
            }
            previewPositions.push(previewPosition);
          } else {
            previewPosition.highlight.push(contentPosition.highlight);
            previewPosition.previewEnd = contentPosition.previewEnd;
            previewPosition.ellipsesAfter = contentPosition.ellipsesAfter;
          }
        }

        var resultPreviews = document.createElement('div');
        resultPreviews.classList.add('search-result-previews');
        resultLink.appendChild(resultPreviews);

        var content = doc.content;
        for (var j = 0; j < Math.min(previewPositions.length, 3); j++) {
          var position = previewPositions[j];

          var resultPreview = document.createElement('div');
          resultPreview.classList.add('search-result-preview');
          resultPreviews.appendChild(resultPreview);

          if (position.ellipsesBefore) {
            resultPreview.appendChild(document.createTextNode('... '));
          }
          addHighlightedText(resultPreview, content, position.previewStart, position.previewEnd, position.highlight);
          if (position.ellipsesAfter) {
            resultPreview.appendChild(document.createTextNode(' ...'));
          }
        }
      }
      var resultRelUrl = document.createElement('span');
      resultRelUrl.classList.add('search-result-rel-url');
      resultRelUrl.innerText = doc.relUrl;
      resultTitle.appendChild(resultRelUrl);
    }

    function addHighlightedText(parent, text, start, end, positions) {
      var index = start;
      for (var i in positions) {
        var position = positions[i];
        var span = document.createElement('span');
        span.innerHTML = text.substring(index, position[0]);
        parent.appendChild(span);
        index = position[0] + position[1];
        var highlight = document.createElement('span');
        highlight.classList.add('search-result-highlight');
        highlight.innerHTML = text.substring(position[0], index);
        parent.appendChild(highlight);
      }
      var span = document.createElement('span');
      span.innerHTML = text.substring(index, end);
      parent.appendChild(span);
    }
  }

  jtd.addEvent(searchInput, 'focus', function(){
    setTimeout(update, 0);
  });

  jtd.addEvent(searchInput, 'keyup', function(e){
    switch (e.keyCode) {
      case 27: // When esc key is pressed, hide the results and clear the field
        searchInput.value = '';
        break;
      case 38: // arrow up
      case 40: // arrow down
      case 13: // enter
        e.preventDefault();
        return;
    }
    update();
  });

  jtd.addEvent(searchInput, 'keydown', function(e){
    switch (e.keyCode) {
      case 38: // arrow up
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          active.classList.remove('active');
          if (active.parentElement.previousSibling) {
            var previous = active.parentElement.previousSibling.querySelector('.search-result');
            previous.classList.add('active');
          }
        }
        return;
      case 40: // arrow down
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          if (active.parentElement.nextSibling) {
            var next = active.parentElement.nextSibling.querySelector('.search-result');
            active.classList.remove('active');
            next.classList.add('active');
          }
        } else {
          var next = document.querySelector('.search-result');
          if (next) {
            next.classList.add('active');
          }
        }
        return;
      case 13: // enter
        e.preventDefault();
        var active = document.querySelector('.search-result.active');
        if (active) {
          active.click();
        } else {
          var first = document.querySelector('.search-result');
          if (first) {
            first.click();
          }
        }
        return;
    }
  });

  jtd.addEvent(document, 'click', function(e){
    if (e.target != searchInput) {
      hideSearch();
    }
  });
}

// Switch theme

jtd.getTheme = function() {
  var cssFileHref = document.querySelector('[rel="stylesheet"]').getAttribute('href');
  return cssFileHref.substring(cssFileHref.lastIndexOf('-') + 1, cssFileHref.length - 4);
}

jtd.setTheme = function(theme) {
  var cssFile = document.querySelector('[rel="stylesheet"]');
  cssFile.setAttribute('href', 'http://localhost:4000/net_10base_t1s/assets/css/just-the-docs-' + theme + '.css');
}

// Document ready

jtd.onReady(function(){
  initNav();
  initSearch();
});

})(window.jtd = window.jtd || {});


