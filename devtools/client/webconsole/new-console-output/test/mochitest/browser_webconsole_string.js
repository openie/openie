/* -*- indent-tabs-mode: nil; js-indent-level: 2 -*- */
/* vim: set ft=javascript ts=2 et sw=2 tw=80: */
/* Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/ */

"use strict";

const TEST_URI = "http://example.com/browser/devtools/client/webconsole/" +
                 "new-console-output/test/mochitest/test-console.html";

add_task(async function() {
  let hud = await openNewTabAndConsole(TEST_URI);

  info("console.log with a string argument");
  let receivedMessages = waitForMessages({
    hud,
    messages: [{
      // Test that the output does not include quotes.
      text: "stringLog",
    }],
  });

  await ContentTask.spawn(gBrowser.selectedBrowser, {}, function() {
    content.wrappedJSObject.stringLog();
  });

  await receivedMessages;

  info("evaluating a string constant");
  let jsterm = hud.jsterm;
  await jsterm.execute("\"string\\nconstant\"");
  let msg = await waitFor(() => findMessage(hud, "constant"));
  let body = msg.querySelector(".message-body");
  // On the other hand, a string constant result should be quoted, but
  // newlines should be let through.
  ok(body.textContent.includes("\"string\nconstant\""), "found expected text");
});
