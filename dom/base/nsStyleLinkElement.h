/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * A base class which implements nsIStyleSheetLinkingElement and can
 * be subclassed by various content nodes that want to load
 * stylesheets (<style>, <link>, processing instructions, etc).
 */

#ifndef nsStyleLinkElement_h___
#define nsStyleLinkElement_h___

#include "mozilla/Attributes.h"
#include "mozilla/CORSMode.h"
#include "mozilla/StyleSheetInlines.h"
#include "mozilla/net/ReferrerPolicy.h"
#include "nsCOMPtr.h"
#include "nsIStyleSheetLinkingElement.h"
#include "nsTArray.h"
#include "nsAttrValue.h"

class nsIDocument;
class nsIURI;

namespace mozilla {
class CSSStyleSheet;
namespace dom {
class ShadowRoot;
} // namespace dom
} // namespace mozilla

class nsStyleLinkElement : public nsIStyleSheetLinkingElement
{
public:
  nsStyleLinkElement();
  virtual ~nsStyleLinkElement();

  NS_IMETHOD QueryInterface(REFNSIID aIID, void** aInstancePtr) override = 0;

  mozilla::StyleSheet* GetSheet() const { return mStyleSheet; }

  // nsIStyleSheetLinkingElement
  void SetStyleSheet(mozilla::StyleSheet* aStyleSheet) override;
  mozilla::StyleSheet* GetStyleSheet() override;
  void InitStyleLinkElement(bool aDontLoadStyle) override;
  nsresult UpdateStyleSheet(nsICSSLoaderObserver* aObserver,
                            bool* aWillNotify,
                            bool* aIsAlternate,
                            bool aForceReload) override;
  void SetEnableUpdates(bool aEnableUpdates) override;
  void GetCharset(nsAString& aCharset) override;

  virtual void OverrideBaseURI(nsIURI* aNewBaseURI) override;
  virtual void SetLineNumber(uint32_t aLineNumber) override;
  virtual uint32_t GetLineNumber() override;

  enum RelValue {
    ePREFETCH =     0x00000001,
    eDNS_PREFETCH = 0x00000002,
    eSTYLESHEET =   0x00000004,
    eNEXT =         0x00000008,
    eALTERNATE =    0x00000010,
    ePRECONNECT =   0x00000020,
    // NOTE: 0x40 is unused
    ePRELOAD =      0x00000080
  };

  // The return value is a bitwise or of 0 or more RelValues.
  static uint32_t ParseLinkTypes(const nsAString& aTypes);

  static bool CheckPreloadAttrs(const nsAttrValue& aAs, const nsAString& aType,
                                const nsAString& aMedia, nsIDocument* aDocument);

  void UpdateStyleSheetInternal()
  {
    UpdateStyleSheetInternal(nullptr, nullptr);
  }
protected:
  /**
   * @param aOldDocument should be non-null only if we're updating because we
   *                     removed the node from the document.
   * @param aForceUpdate true will force the update even if the URI has not
   *                     changed.  This should be used in cases when something
   *                     about the content that affects the resulting sheet
   *                     changed but the URI may not have changed.
   */
  nsresult UpdateStyleSheetInternal(nsIDocument *aOldDocument,
                                    mozilla::dom::ShadowRoot *aOldShadowRoot,
                                    bool aForceUpdate = false);

  virtual already_AddRefed<nsIURI> GetStyleSheetURL(bool* aIsInline, nsIPrincipal** aTriggeringPrincipal) = 0;
  virtual void GetStyleSheetInfo(nsAString& aTitle,
                                 nsAString& aType,
                                 nsAString& aMedia,
                                 bool* aIsAlternate) = 0;

  virtual mozilla::CORSMode GetCORSMode() const
  {
    // Default to no CORS
    return mozilla::CORS_NONE;
  }

  virtual mozilla::net::ReferrerPolicy GetLinkReferrerPolicy()
  {
    return mozilla::net::RP_Unset;
  }

  // CC methods
  void Unlink();
  void Traverse(nsCycleCollectionTraversalCallback &cb);

private:
  /**
   * @param aOldDocument should be non-null only if we're updating because we
   *                     removed the node from the document.
   * @param aOldShadowRoot The ShadowRoot that used to contain the style.
   *                     Passed as a parameter because on an update, the node
   *                     is removed from the tree before the sheet is removed
   *                     from the ShadowRoot.
   * @param aForceUpdate true will force the update even if the URI has not
   *                     changed.  This should be used in cases when something
   *                     about the content that affects the resulting sheet
   *                     changed but the URI may not have changed.
   */
  nsresult DoUpdateStyleSheet(nsIDocument* aOldDocument,
                              mozilla::dom::ShadowRoot* aOldShadowRoot,
                              nsICSSLoaderObserver* aObserver,
                              bool* aWillNotify,
                              bool* aIsAlternate,
                              bool aForceUpdate);

  RefPtr<mozilla::StyleSheet> mStyleSheet;
protected:
  nsCOMPtr<nsIPrincipal> mTriggeringPrincipal;
  bool mDontLoadStyle;
  bool mUpdatesEnabled;
  uint32_t mLineNumber;
};

#endif /* nsStyleLinkElement_h___ */

